#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "main.h"
#include "w5500_setup.h"
#include "log_handler.h"
#include "w5500_driver.h"

SPI_HandleTypeDef *wiznet_hspi;
TIM_HandleTypeDef *wiznet_htim;

// Default network configuration
wiz_NetInfo net_info = {
    //.ip = {10, 0, 0, 69},
		//.gw = {10, 0, 0, 1},
		.ip = {192, 168, 137, 100},
		.gw = {192, 168, 137, 1},
    .sn = {255, 255, 255, 0},
};

/**
 * @brief Initialize W5500 chip with new driver
 * @param hspi SPI handle for W5500 communication
 * @param htim Timer handle for periodic socket polling
 */
void W5500Init(SPI_HandleTypeDef *hspi, TIM_HandleTypeDef *htim) {
		wiznet_hspi = hspi;
    wiznet_htim = htim;
    // Small delay for chip power-up
    osDelay(500);
    
    // Step 1: Configure hardware (SPI, CS pin, and timer)
    int result = setWiznetHardware(wiznet_hspi, SPI1_CS_GPIO_Port, SPI1_CS_Pin, htim);
    if (result != 0) {
        log_msg(LL_ERR, "Failed to set W5500 hardware: %d", result);
        return;
    }
    
    // Step 2: Configure socket buffer sizes
    uint8_t rx_buf_sizes[8] = {0};
    uint8_t tx_buf_sizes[8] = {0};

    // Can choose from values 1, 2, 4, 8, 16
    // Total sum cannot exceed 16 for rx and tx buffers individually

    rx_buf_sizes[DHCP_SOCKET] = 1;
    rx_buf_sizes[DNS_SOCKET] = 1;
    rx_buf_sizes[WS_SOCKET] = 1;
    rx_buf_sizes[NTP_SOCKET] = 1;
    
    tx_buf_sizes[DHCP_SOCKET] = 1;
		tx_buf_sizes[DNS_SOCKET] = 1;
		tx_buf_sizes[WS_SOCKET] = 8;
		tx_buf_sizes[NTP_SOCKET] = 1;

    // Step 3: Initialize W5500 chip with network configuration
    result = initWizchip(net_info.ip, net_info.sn, net_info.gw, 
                        rx_buf_sizes, tx_buf_sizes);
    if (result != 0) {
        log_msg(LL_ERR, "Failed to initialize W5500 chip: %d", result);
        return;
    }
    
    osDelay(500);
    
    log_msg(LL_DBG, "W5500 init successful!");
}
