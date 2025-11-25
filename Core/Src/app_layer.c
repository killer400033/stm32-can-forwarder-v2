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

// WebSocket tx buffer union
typedef union {
    uint8_t full[WS_MAX_HEADER_LEN + 512];  // Full buffer
    struct {
        uint8_t header_space[WS_MAX_HEADER_LEN];  // Reserved for WebSocket header
        uint8_t data[512];                          // Payload data
    };
} ws_send_buffer_t;

static ws_send_buffer_t ws_send_buf;
static CanFrameList_t canFrameList = {0};

// WebSocket TX/RX buffers for socket API
static uint8_t ws_tx_buf[4096+3];
static uint8_t ws_rx_buf[1024+3];

// WebSocket RX buffer from WebSocket client
uint8_t recv_buf[256];
uint16_t recv_len = sizeof(recv_buf);

void appLayerThread(void *argument);
static size_t packetEncode(pb_byte_t *buffer, size_t length, CanFrameList_t *canFrames);
static bool canFramesEncode(pb_ostream_t *stream, const pb_field_iter_t *field, void * const *arg);
static void dnsResolveCallback(const uint8_t* ip_addr, uint32_t ttl, bool success);

extern osMessageQueueId_t wsCanQueueHandle;
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
	uint32_t prev_cnt = 0;
	uint32_t cnt = 0;
	bool connected = false;

	// Initialize WebSocket client with new API
	ws_config_t ws_config = {
		.socket_num = WS_SOCKET,
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

		ws_state_t ws_state = ws_client_get_state();

		// Attempt to connect if disconnected
		if (ws_state == WS_STATE_DISCONNECTED || ws_state == WS_STATE_ERROR) {
			int result = ws_client_connect();
			if (result != WS_OK) {
				log_msg(LL_WRN, "WebSocket connection failed: %d", result);
				osDelay(5000);
			}
		}

		// Handle state transitions
		if (ws_state == WS_STATE_CONNECTED && !connected) {
			log_msg(LL_DBG, "WebSocket connected!");
			connected = true;
		}
		if ((ws_state == WS_STATE_DISCONNECTED || ws_state == WS_STATE_ERROR) && connected) {
			log_msg(LL_WRN, "WebSocket disconnected...");
			connected = false;
		}

		// Process WebSocket (handles callbacks, state updates, receives data)
		ws_opcode_t opcode;
		int8_t proc_result = ws_client_process(recv_buf, &recv_len, &opcode);
		
		if (proc_result < 0) {
			// Error occurred
			if (proc_result == WS_ERR_TIMEOUT) {
				log_msg(LL_WRN, "WebSocket timeout");
			} else if (proc_result == WS_ERR_DISCONNECTED) {
				log_msg(LL_DBG, "WebSocket disconnected gracefully");
			}
		} else if (proc_result > 0) {
			// Data received (not currently used, but available)
			log_msg(LL_DBG, "WS RX: %d bytes, opcode: %d", recv_len, opcode);
		}

		// Send data if connected
		if (ws_state == WS_STATE_CONNECTED) {
			// Collect CAN frames from queue
			while (osMessageQueueGetCount(wsCanQueueHandle) > 0 && canFrameList.count < MAX_CAN_FRAME_CNT) {
				osMessageQueueGet(wsCanQueueHandle, &(canFrameList.canFrames[canFrameList.count++]), 0, 0);
			}

			if (canFrameList.count > 0) {
				// Encode packet into the data section of the buffer
				size_t msg_length = packetEncode(ws_send_buf.data, sizeof(ws_send_buf.data), &canFrameList);

				// Send data over WebSocket (function will write header in header_space)
				if (msg_length > 0) {
					int16_t sent = ws_client_send_binary(ws_send_buf.data, (uint16_t)msg_length);
					if (sent < 0) {
						log_msg(LL_ERR, "WebSocket send failed: %d", sent);
					} else {
						cnt += canFrameList.count;
					}
				}
				
				canFrameList.count = 0;
			}
		}

		// Throughput measuring
		if (osKernelGetTickCount() - prev_time > 10000) {
			uint32_t speed = (cnt - prev_cnt) * sizeof(CanFrame) / 10;
			prev_time = osKernelGetTickCount();
			prev_cnt = cnt;
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
