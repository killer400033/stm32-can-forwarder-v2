#include "stream.h"
#include "cmsis_os.h"
#include "forwarder_pb.pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "udp_client.h"
#include "w5500_setup.h"
#include "log_handler.h"
#include "dns_resolve.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

osThreadId_t streamTaskHandle;
const osThreadAttr_t streamTask_attributes = {
  .name = "UDP Stream",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

// Maximum number of CAN frames to send in one UDP packet
#define MAX_CAN_FRAME_CNT 5

typedef struct CanFrameList {
    CanFrame canFrames[MAX_CAN_FRAME_CNT];
    uint8_t count;
} CanFrameList_t;

// UDP buffer for encoded data
static uint8_t udp_send_buf[2048];
static CanFrameList_t canFrameList = {0};

// UDP TX/RX buffers for socket API
static uint8_t udp_tx_buf[4096+3];
static uint8_t udp_rx_buf[8192+3];

static uint32_t dns_ttl_expiry = 0;  // Timestamp when TTL expires
static bool dns_request_pending = false;
static bool has_ip = false;

void streamThread(void *argument);
static size_t packetEncode(pb_byte_t *buffer, size_t length, CanFrameList_t *canFrames);
static bool canFramesEncode(pb_ostream_t *stream, const pb_field_iter_t *field, void * const *arg);
static void dnsResolveCallback(const uint8_t* ip_addr, uint32_t ttl, bool success);

extern osMessageQueueId_t wsCanQueueHandle;
extern osMessageQueueId_t dnsReqQueueHandle;

void initStream(void) {
	streamTaskHandle = osThreadNew(streamThread, NULL, &streamTask_attributes);
}

// Main thread that runs
void streamThread(void *argument) {
	uint32_t prev_time = osKernelGetTickCount();
	uint32_t cnt = 0;
	bool initialized = false;

	// Initialize UDP client configuration
	udp_client_config_t udp_config = {
		.socket_num = STREAM_SOCKET,
		.source_port = 5000,  // Local source port
		.dest_ip = {0, 0, 0, 0},
		.dest_port = STREAM_UDP_PORT,
		.tx_buf = udp_tx_buf,
		.tx_buf_len = sizeof(udp_tx_buf),
		.rx_buf = udp_rx_buf,
		.rx_buf_len = sizeof(udp_rx_buf)
	};

	// Initialize UDP client
	int8_t result = udp_client_init(&udp_config);
	if (result != UDP_OK) {
		log_msg(LL_ERR, "UDP client init failed: %d", result);
	} else {
		log_msg(LL_DBG, "UDP streaming initialized");
		initialized = true;
	}

	for (;;) {
		if (!initialized) {
            log_msg(LL_ERR, "UDP streaming not initialized, exiting thread");
			osThreadExit();
		}

        if (!dns_request_pending && (!has_ip || osKernelGetTickCount() > dns_ttl_expiry)) {
			// Check if queue has space
			if (osMessageQueueGetSpace(dnsReqQueueHandle) > 0) {
				dns_request_t dns_req = {
					.domain_name = STREAM_DOMAIN_NAME,
					.callback = dnsResolveCallback
				};
				
				if (osMessageQueuePut(dnsReqQueueHandle, &dns_req, 0, 0) == osOK) {
					dns_request_pending = true;
				}
			}
		}

        // Wait for valid IP before attempting connection
		if (!has_ip) {
			osDelay(1000);
			continue;
		}

		// Collect CAN frames from queue
		while (osMessageQueueGetCount(wsCanQueueHandle) > 0 && canFrameList.count < MAX_CAN_FRAME_CNT) {
			osMessageQueueGet(wsCanQueueHandle, &(canFrameList.canFrames[canFrameList.count++]), 0, 0);
		}

		if (canFrameList.count > 0) {
			// Encode packet into the buffer
			size_t msg_length = packetEncode(udp_send_buf, sizeof(udp_send_buf), &canFrameList);

			// Send data over UDP
			if (msg_length > 0) {
				result = udp_client_send(udp_send_buf, (uint16_t)msg_length);
				if (result == UDP_OK) {
					cnt += canFrameList.count;
					canFrameList.count = 0;
				}
			}
		}

		// Throughput measuring
		if (osKernelGetTickCount() - prev_time > 10000) {
			uint32_t speed = cnt * sizeof(CanFrame) / 10;
			prev_time = osKernelGetTickCount();
			cnt = 0;
			log_msg(LL_DBG, "UDP Stream: %ld B/s", speed);
		}

		osThreadYield();
	}
}


// Nanopb encode function to encode Packet
static size_t packetEncode(pb_byte_t *buffer, size_t length, CanFrameList_t *canFrames) {
	// Setup pack message
	Packet packet = Packet_init_zero;
	// stream to write buffer
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	packet.canFrames.arg = canFrames;
	packet.canFrames.funcs.encode = canFramesEncode;

	//encode the pack
	bool status = pb_encode(&stream, Packet_fields, &packet);
	if (!status) {
		log_msg(LL_ERR, "Encoding failed: %s", PB_GET_ERROR(&stream));
		return 0;
	}
	return stream.bytes_written;
}

// Nanopb callback to encode one CanFrame in Packet
static bool canFramesEncode(pb_ostream_t *stream, const pb_field_iter_t *field, void * const *arg) {
	CanFrameList_t *source = (CanFrameList_t*)(*arg);

	for(int i=0; i < source->count; i++) {
		if (!pb_encode_tag_for_field(stream, field)) {
			log_msg(LL_ERR, "CanFrame tag encode error: %s", PB_GET_ERROR(stream));
			return false;
		}
		if (!pb_encode_submessage(stream, CanFrame_fields, &(source->canFrames[i]))) {
			log_msg(LL_ERR, "CanFrame body encode error: %s", PB_GET_ERROR(stream));
			return false;
		}
	}
	return true;
}

// DNS resolution callback
static void dnsResolveCallback(const uint8_t* ip_addr, uint32_t ttl, bool success) {
	if (success && ip_addr != NULL) {
		udp_client_set_dest_ip(ip_addr);
		has_ip = true;
		// Set expiry time: current time + TTL (in milliseconds)
		dns_ttl_expiry = osKernelGetTickCount() + (ttl * 1000);
	}
	dns_request_pending = false;
}
