#include "stream.h"
#include "cmsis_os.h"
#include "forwarder_pb.pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "websocket_client.h"
#include "w5500_setup.h"
#include "storage.h"
#include "dns_client.h"
#include "dns_resolve.h"
#include "log_handler.h"
#include "unix_time.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "w5500_driver.h"
#include "can_driver.h"

osThreadId_t streamTaskHandle;
const osThreadAttr_t streamTask_attributes = {
  .name = "Stream",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

// Maximum number of CAN frames to send in one websocket packet
#define MAX_CAN_FRAME_CNT 40

typedef struct CanFrameList {
    CanFrame canFrames[MAX_CAN_FRAME_CNT];
    uint8_t count;
} CanFrameList_t;

// WebSocket tx buffer union
typedef union {
    uint8_t full[WS_MAX_HEADER_LEN + 2048];  // Full buffer
    struct {
        uint8_t header_space[WS_MAX_HEADER_LEN];  // Reserved for WebSocket header
        uint8_t data[2048];                       // Payload data
    };
} ws_send_buffer_t;

static uint8_t ws_recv_buf[2048];
static ws_send_buffer_t ws_send_buf;
static CanFrameList_t canFrameListTx = {0};
static CanFrame canFrameRx = {0};

// WebSocket TX/RX buffers for socket API
static uint8_t ws_tx_buf[4096+3];
static uint8_t ws_rx_buf[2048+3];

void streamTask(void *argument);
static size_t packetEncode(pb_byte_t *buffer, size_t length, CanFrameList_t *canFrames);
static bool canFramesEncode(pb_ostream_t *stream, const pb_field_iter_t *field, void * const *arg);
static bool canFrameDecode(const pb_byte_t *buffer, size_t length, CanFrame *frame);
static void dnsResolveCallback(const uint8_t* ip_addr, uint32_t ttl, bool success);

extern osMessageQueueId_t canStreamQueueHandle;
extern osMessageQueueId_t dnsReqQueueHandle;
extern osMessageQueueId_t networkLogQueueHandle;

// DNS state tracking
static uint8_t ws_server_ip[4] = {0};
static uint32_t dns_ttl_expiry = 0;  // Timestamp when TTL expires
static bool dns_request_pending = false;
static bool has_valid_ip = false;

RNG_HandleTypeDef *app_rng = NULL;

void initStream(RNG_HandleTypeDef *hrng) {
	streamTaskHandle = osThreadNew(streamTask, NULL, &streamTask_attributes);
	app_rng = hrng;
}

// DNS resolution callback
static void dnsResolveCallback(const uint8_t* ip_addr, uint32_t ttl, bool success) {
	if (success && ip_addr != NULL) {
		memcpy(ws_server_ip, ip_addr, 4);
		has_valid_ip = true;
		// Set expiry time: current time + TTL (in milliseconds)
		dns_ttl_expiry = osKernelGetTickCount() + (ttl * 1000);
	}
	dns_request_pending = false;
}

// Main thread that runs
void streamTask(void *argument) {
	uint32_t prev_time = osKernelGetTickCount();
	uint32_t tx_cnt = 0;
	uint32_t rx_cnt = 0;
	bool connected = false;

	// Initialize WebSocket client with new API
	ws_config_t ws_config = {
		.socket_num = STREAM_SOCKET,
		.host = {0, 0, 0, 0},  // Will be set after DNS resolution
		.port = WS_DOMAIN_PORT,
		.path = WS_DOMAIN_PATH,
		.tx_buf = ws_tx_buf,
		.tx_buf_len = sizeof(ws_tx_buf),
		.rx_buf = ws_rx_buf,
		.rx_buf_len = sizeof(ws_rx_buf)
	};

	for (;;) {
		// Check if DNS TTL has expired and no request is pending
		if ((!has_valid_ip || dns_ttl_expiry <= osKernelGetTickCount()) && !dns_request_pending) {
			// Check if queue has space
			if (osMessageQueueGetSpace(dnsReqQueueHandle) > 0) {
				dns_request_t dns_req = {
					.domain_name = WS_DOMAIN_NAME,
					.callback = dnsResolveCallback
				};
				
				if (osMessageQueuePut(dnsReqQueueHandle, &dns_req, 0, 0) == osOK) {
					dns_request_pending = true;
				}
			}
		}

		// Wait for valid IP before attempting connection
		if (!has_valid_ip || !ntp_sync_successful) {
			osDelay(1000);
			continue;
		}

		// Update WebSocket config with resolved IP
		if (has_valid_ip && ws_client_get_state() == WS_STATE_DISCONNECTED && memcmp(ws_config.host, ws_server_ip, 4) != 0 && ntp_sync_successful) {
			memcpy(ws_config.host, ws_server_ip, 4);
			ws_client_init(&ws_config);
		}

		// Attempt to connect if disconnected
		if (ws_client_get_state() == WS_STATE_DISCONNECTED) {
			int result = ws_client_connect();
			if (result != WS_OK) {
				log_msg(LL_WRN, "WebSocket connection failed: %d", result);
				osDelay(5000);
			}
		}

		// Process WebSocket and received data
		ws_opcode_t opcode;
		int16_t len = ws_client_process(ws_recv_buf, sizeof(ws_recv_buf), &opcode);
		if (len > 0 && opcode == WS_OPCODE_BINARY) {
			// Decode CanFrame and send to can peripheral
			if (canFrameDecode(ws_recv_buf, len, &canFrameRx)) {
				sendCanFrame(canFrameRx.can_id, canFrameRx.can_bus, canFrameRx.can_data.bytes, canFrameRx.can_data.size);
				rx_cnt++;
			}
		}

		// Handle state transitions
		if (ws_client_get_state() == WS_STATE_CONNECTED && !connected) {
			log_msg(LL_DBG, "WebSocket Connected!");
			connected = true;
		}
		if ((ws_client_get_state() == WS_STATE_DISCONNECTED) && connected) {
			log_msg(LL_WRN, "WebSocket Disconnected...");
			connected = false;
		}

		// Send data if connected
		if (ws_client_get_state() == WS_STATE_CONNECTED) {
			// Collect CAN frames from queue
			while (osMessageQueueGetCount(canStreamQueueHandle) > 0 && canFrameListTx.count < MAX_CAN_FRAME_CNT) {
				osMessageQueueGet(canStreamQueueHandle, &(canFrameListTx.canFrames[canFrameListTx.count++]), 0, 0);
			}

			if (canFrameListTx.count > 0) {
				// Encode packet into the data section of the buffer
				size_t msg_length = packetEncode(ws_send_buf.data, sizeof(ws_send_buf.data), &canFrameListTx);

				// Send data over WebSocket (function will write header in header_space)
				if (msg_length > 0) {
					if (ws_client_send_binary(ws_send_buf.data, (uint16_t)msg_length) > 0) {
						tx_cnt += canFrameListTx.count * sizeof(CanFrame);
						canFrameListTx.count = 0;
					}
				}
			}

			// Send Logs
			log_queue_entry_t logEntry;
			while (osMessageQueueGetCount(networkLogQueueHandle) > 0) {
				if (osMessageQueueGet(networkLogQueueHandle, &logEntry, 0, 0) == osOK) {
					uint16_t len = logEntry.length > sizeof(ws_send_buf.data) ? sizeof(ws_send_buf.data) : logEntry.length;
					memcpy(ws_send_buf.data, logEntry.start_ptr, len);

					if (ws_client_send_text(ws_send_buf.data, len) > 0) {
						tx_cnt += len;
					}
				}
			}
		}

		// Throughput measuring
		if (osKernelGetTickCount() - prev_time > 10000) {
			uint32_t speed = tx_cnt / 10;
			prev_time = osKernelGetTickCount();
			tx_cnt = 0;
			log_msg(LL_DBG, "Websocket TX: %ld B/s", speed);
			log_msg(LL_DBG, "Websocket RX: %ld Frames", rx_cnt);
			rx_cnt = 0;
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

static bool canFrameDecode(const pb_byte_t *buffer, size_t length, CanFrame *frame) {
    pb_istream_t stream = pb_istream_from_buffer(buffer, length);
    *frame = (CanFrame)CanFrame_init_zero;

    if (!pb_decode(&stream, CanFrame_fields, frame)) {
        log_msg(LL_ERR, "CanFrame decode error: %s", PB_GET_ERROR(&stream));
        return false;
    }
    return true;
}

uint32_t ws_rand(void) {
	uint32_t random_value;
	HAL_RNG_GenerateRandomNumber(app_rng, &random_value);
	return random_value;
}
