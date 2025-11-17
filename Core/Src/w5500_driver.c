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

extern SPI_HandleTypeDef hspi1;

wiz_NetInfo net_info = {
	.mac = { 0x00, 0x08, 0xDC, 0xAB, 0xCD, 0xEF },
  .gw = {10, 0, 0, 1},
	.ip = {10, 0, 0, 69},
	//.gw = {192, 168, 137, 1},
	//.ip = {192, 168, 137, 100},
	.sn = {255, 255, 255, 0},
	.dns = {8, 8, 8, 8},
	.dhcp = NETINFO_STATIC,
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
	osDelay(500);
	// Register W5500 callbacks
	reg_wizchip_cs_cbfunc(wizchipSelect, wizchipUnselect);
	reg_wizchip_spi_cbfunc(wizchipReadByte, wizchipWriteByte);

	// Set W5500 Socket buffer sizes
	uint8_t rx_tx_buff_sizes[2][8] = { {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2} };
	ctlwizchip(CW_INIT_WIZCHIP, (void*) rx_tx_buff_sizes);
    ctlnetwork(CN_SET_NETINFO, (void*) &net_info);

    osDelay(500);
}


void sendData(uint8_t* pBuf, uint16_t len) {
	HAL_SPI_Transmit(&hspi1, pBuf, len, HAL_MAX_DELAY);
}
