#include "app_layer.h"
#include "cmsis_os.h"
#include "forwarder_pb.pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "websocket_client.h"
#include "w5500_driver.h"
#include "storage.h"
#include "dns_client.h"
#include "dns_resolve.h"
#include "log_handler.h"
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

pb_byte_t buffer[512]; // Buffer *should* be enough, might need to be increased if string in buffer is increased in length
volatile uint32_t dropped_packets = 0;

void appLayerThread(void *argument);
static size_t packetEncode(pb_byte_t *buffer, size_t length, CanFrameList_t *canFrameList);
static bool canFramesEncode(pb_ostream_t *stream, const pb_field_iter_t *field, void * const *arg);
static void dnsResolveCallback(const uint8_t* ip_addr, uint32_t ttl, bool success);

extern osMessageQueueId_t wsCanQueueHandle;
extern osMessageQueueId_t dnsReqQueueHandle;

// DNS configuration
#define WS_DOMAIN_NAME "ws-alb-1889248813.ap-southeast-2.elb.amazonaws.com"

// DNS state tracking
static uint8_t ws_server_ip[4] = {0};
static uint32_t dns_ttl_expiry = 0;  // Timestamp when TTL expires
static bool dns_request_pending = false;
static bool has_valid_ip = false;

void initAppLayer() {
	appLayerTaskHandle = osThreadNew(appLayerThread, NULL, &appLayerTask_attributes);
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
	CanFrameList_t canFrameList = {0};
	uint32_t prev_time = osKernelGetTickCount();
	uint32_t prev_cnt = 0;
	uint32_t cnt = 0;

	static websocket_client_t ws_client;
	static bool connected = false;

	ws_client_init(&ws_client, WS_SOCKET);

	for (;;) {
		if (ws_client.state == WS_STATE_CONNECTED && !connected) {
			log_msg(LL_DBG, "WebSocket connected!");
			connected = true;
	    }
		if (ws_client.state == WS_STATE_DISCONNECTED && connected) {
			log_msg(LL_WRN, "WebSocket disconnected...");
			connected = false;
		}
		ws_client_process(&ws_client);

		// Check if DNS TTL has expired and no request is pending
		uint32_t current_time = osKernelGetTickCount();
		if ((!has_valid_ip || current_time >= dns_ttl_expiry) && !dns_request_pending) {
			// Check if queue has space
			if (osMessageQueueGetSpace(dnsReqQueueHandle) > 0) {
				dns_request_t dns_req = {
					.domain_name = WS_DOMAIN_NAME,
					.callback = dnsResolveCallback
				};
				
				if (osMessageQueuePut(dnsReqQueueHandle, &dns_req, 0, 0) == osOK) {
					dns_request_pending = true;
				} else {
					log_msg(LL_ERR, "DNS Request: Failed to queue request");
					osDelay(1000);
				}
			} else {
				log_msg(LL_ERR, "DNS Request: Queue full, cannot queue request");
				osDelay(1000);
			}
		}

		// Wait for valid IP before attempting connection
		if (!has_valid_ip) {
			osDelay(1000);
			continue;
		}

		if (ws_client.state == WS_STATE_DISCONNECTED) {
			if (ws_client_connect(&ws_client, ws_server_ip, 80, "/api/decoder/connection") != 0) {
				log_msg(LL_WRN, "WebSocket client connection failed...");
			}
		}

		if (ws_client.state == WS_STATE_CONNECTED) {
			while (osMessageQueueGetCount(wsCanQueueHandle) > 0 && canFrameList.count < MAX_CAN_FRAME_CNT) {
				osMessageQueueGet(wsCanQueueHandle, &(canFrameList.canFrames[canFrameList.count++]), 0, 0);
			}

			size_t msg_length = packetEncode(buffer, sizeof(buffer), &canFrameList);

			// Send data over WebSocket
			if (ws_client.state == WS_STATE_CONNECTED && msg_length > 0) {
				(void)ws_client_send_binary(&ws_client, buffer, (uint16_t)msg_length);
			}
			cnt += canFrameList.count;
			canFrameList.count = 0;
		}

		// Throughput measuring
		if (osKernelGetTickCount() - prev_time > 10000) {
			uint32_t speed = (cnt - prev_cnt) * sizeof(CanFrame) / 10;
			prev_time = osKernelGetTickCount();
			prev_cnt = cnt;
			log_msg(LL_DBG, "Websocket: %ld B/s", speed);
			log_msg(LL_DBG, "Connect Stat: %d", ws_client.state);
			log_msg(LL_DBG, "Dropped Packets: %ld", dropped_packets);
		}

		osThreadYield();
	}
}


// Nanopb encode function to encode Packet
static size_t packetEncode(pb_byte_t *buffer, size_t length, CanFrameList_t *canFrameList) {
	// Setup pack message
	Packet packet = Packet_init_zero;
	// stream to write buffer
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	packet.canFrames.arg = canFrameList;
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
