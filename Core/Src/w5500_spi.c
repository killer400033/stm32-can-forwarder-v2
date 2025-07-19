#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "main.h"
#include "dhcp.h"
#include "httpServer.h"
#include "socket.h"
#include "w5500.h"
#include "websocket_client.h"

#define DHCP_SOCKET     0
#define DNS_SOCKET      1
#define HTTP_SOCKET     2
#define MAX_HTTPSOCK    6

extern SPI_HandleTypeDef hspi1;

wiz_NetInfo net_info = {
	.mac = { 0x00, 0x08, 0xDC, 0xAB, 0xCD, 0xEF },
    .gw = {192, 168, 137, 1},
	.ip = {192, 168, 137, 100},
	.sn = {255, 255, 255, 0},
	.dns = {8, 8, 8, 8},
	.dhcp = 1,
};

void wizchipSelect(void) {
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
}

void wizchipUnselect(void) {
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void wizchipReadBurst(uint8_t* buff, uint16_t len) {
    HAL_SPI_Receive(&hspi1, buff, len, HAL_MAX_DELAY);
}

void wizchipWriteBurst(uint8_t* buff, uint16_t len) {
    HAL_SPI_Transmit(&hspi1, buff, len, HAL_MAX_DELAY);
}

uint8_t wizchipReadByte(void) {
    uint8_t byte;
    wizchipReadBurst(&byte, sizeof(byte));
    return byte;
}

void wizchipWriteByte(uint8_t byte) {
    wizchipWriteBurst(&byte, sizeof(byte));
}

void W5500Init() {
	HAL_Delay(500);
	// Register W5500 callbacks
	reg_wizchip_cs_cbfunc(wizchipSelect, wizchipUnselect);
	reg_wizchip_spi_cbfunc(wizchipReadByte, wizchipWriteByte);

	// Set W5500 Socket buffer sizes
	uint8_t rx_tx_buff_sizes[2][8] = { {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2} };
	ctlwizchip(CW_INIT_WIZCHIP, (void*) rx_tx_buff_sizes);
    ctlnetwork(CN_SET_NETINFO, (void*) &net_info);
}

#define PACKET_SIZE 1000
uint8_t test_data[PACKET_SIZE] = {0};

void startServer() {
  // Create WebSocket client
  websocket_client_t ws_client;
  if (ws_client_init(&ws_client, 2) != 0) {
      printf("WebSocket client initialization failed!\n");
      return;
  }

  // Connect to WebSocket server
  printf("Connecting to WebSocket server for speed test...\n");
  HAL_Delay(1000);
  if (ws_client_connect(&ws_client, "192.168.137.1", 8081, "/") != 0) {
      printf("Failed to initiate WebSocket connection!\n");
      return;
  }

  // Main speed test loop
  bool connected = false;
  bool test_running = false;
  uint32_t next_stats_print = 0;
  float total_bytes_sent = 0;

  while (1) {
      // Process WebSocket state machine
      ws_client_process(&ws_client);

      // Check connection status
      if (ws_client.state == WS_STATE_CONNECTED && !connected) {
          printf("WebSocket connected! Starting speed test...\n");
          printf("Sending 100-byte messages as fast as possible\n");
          connected = true;
          test_running = true;
      }

      if (ws_client.state == WS_STATE_ERROR) {
          printf("WebSocket error occurred!\n");
          break;
      }

      if (ws_client.state == WS_STATE_DISCONNECTED) {
          printf("WebSocket disconnected!\n");
          break;
      }

      // Send messages as fast as possible
      if (test_running) {
          // Try to send a message
          if (ws_client_send_binary(&ws_client, test_data, PACKET_SIZE) == 0) {
              total_bytes_sent += PACKET_SIZE; // Each message is exactly 100 bytes
          }

          // Print statistics every 5 seconds
          uint32_t current_time = HAL_GetTick();
          if (current_time >= next_stats_print) {
          	total_bytes_sent = total_bytes_sent / 5000.0f;
						printf("%.1f kB/s\n", total_bytes_sent);
						total_bytes_sent = 0;
						next_stats_print = current_time + 5000;
          }
      }
  }

  // Clean up
  ws_client_close(&ws_client);
}

void sendData(uint8_t* pBuf, uint16_t len) {
	//HAL_SPI_Transmit(&hspi1, pBuf, len, HAL_MAX_DELAY);
	HAL_SPI_Transmit_DMA(&hspi1, pBuf, len);
	HAL_Delay(100);
}
