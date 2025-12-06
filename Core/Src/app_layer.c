#include "app_layer.h"
#include "cmsis_os.h"
#include "forwarder_pb.pb.h"
#include "pb_common.h"
#include "pb_encode.h"
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

osThreadId_t appLayerTaskHandle;
const osThreadAttr_t appLayerTask_attributes = {
  .name = "App Layer",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

// Maximum number of CAN frames to send in one websocket packet
#define MAX_CAN_FRAME_CNT 9

typedef struct CanFrameList {
    CanFrame canFrames[MAX_CAN_FRAME_CNT];
    uint8_t count;
} CanFrameList_t;

// WebSocket tx buffer union
typedef union {
    uint8_t full[WS_MAX_HEADER_LEN + 1024];  // Full buffer
    struct {
        uint8_t header_space[WS_MAX_HEADER_LEN];  // Reserved for WebSocket header
        uint8_t data[1024];                       // Payload data
    };
} ws_send_buffer_t;

static ws_send_buffer_t ws_send_buf;
static CanFrameList_t canFrameList = {0};

// WebSocket TX/RX buffers for socket API
static uint8_t ws_tx_buf[4096+3];
static uint8_t ws_rx_buf[1024+3];

void appLayerThread(void *argument);
static size_t packetEncode(pb_byte_t *buffer, size_t length, CanFrameList_t *canFrames);
static bool canFramesEncode(pb_ostream_t *stream, const pb_field_iter_t *field, void * const *arg);
static void dnsResolveCallback(const uint8_t* ip_addr, uint32_t ttl, bool success);

extern osMessageQueueId_t canStreamQueueHandle;
extern osMessageQueueId_t dnsReqQueueHandle;

// DNS state tracking
static uint8_t ws_server_ip[4] = {0};
static uint32_t dns_ttl_expiry = 0;  // Timestamp when TTL expires
static bool dns_request_pending = false;
static bool has_valid_ip = false;

RNG_HandleTypeDef *app_rng = NULL;

void initAppLayer(RNG_HandleTypeDef *hrng) {
	appLayerTaskHandle = osThreadNew(appLayerThread, NULL, &appLayerTask_attributes);
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
void appLayerThread(void *argument) {
	uint32_t prev_time = osKernelGetTickCount();
	uint32_t cnt = 0;
	bool connected = false;

	// Initialize WebSocket client with new API
	ws_config_t ws_config = {
		.socket_num = STREAM_SOCKET,
		.host = {0, 0, 0, 0},  // Will be set after DNS resolution
		.port = 80,
		.path = WS_DOMAIN_PATH,
		.tx_buf = ws_tx_buf,
		.tx_buf_len = sizeof(ws_tx_buf),
		.rx_buf = ws_rx_buf,
		.rx_buf_len = sizeof(ws_rx_buf)
	};

	for (;;) {
		// Check if DNS TTL has expired and no request is pending
		if (!has_valid_ip && !dns_request_pending) {
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
		if (!has_valid_ip) {
			osDelay(1000);
			continue;
		}

		// Update WebSocket config with resolved IP (only needs to be done once)
		if (has_valid_ip && ws_config.host[0] == 0) {
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

		// Process WebSocket
		ws_opcode_t opcode;
		ws_client_process(NULL, 0, &opcode);

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
			while (osMessageQueueGetCount(canStreamQueueHandle) > 0 && canFrameList.count < MAX_CAN_FRAME_CNT) {
				osMessageQueueGet(canStreamQueueHandle, &(canFrameList.canFrames[canFrameList.count++]), 0, 0);
			}

			if (canFrameList.count > 0) {
				// Encode packet into the data section of the buffer
				size_t msg_length = packetEncode(ws_send_buf.data, sizeof(ws_send_buf.data), &canFrameList);

				// Send data over WebSocket (function will write header in header_space)
				if (msg_length > 0) {
					if (ws_client_send_binary(ws_send_buf.data, (uint16_t)msg_length) > 0) {
						cnt += canFrameList.count;
						canFrameList.count = 0;
					}
				}
			}
		}

		// Throughput measuring
		if (osKernelGetTickCount() - prev_time > 10000) {
			uint32_t speed = cnt * sizeof(CanFrame) / 10;
			prev_time = osKernelGetTickCount();
			cnt = 0;
			log_msg(LL_DBG, "Websocket: %ld B/s", speed);
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

uint32_t ws_rand(void) {
	uint32_t random_value;
	HAL_RNG_GenerateRandomNumber(app_rng, &random_value);
	return random_value;
}
