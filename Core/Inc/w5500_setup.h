#ifndef INC_W5500_SETUP_H_
#define INC_W5500_SETUP_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

extern SPI_HandleTypeDef *wiznet_hspi;
extern TIM_HandleTypeDef *wiznet_htim;

// Socket assignments
#define DHCP_SOCKET     0
#define DNS_SOCKET      1
#define WS_SOCKET       2
#define NTP_SOCKET      3
#define MAX_HTTPSOCK    6

// Network configuration structure
typedef struct {
    uint8_t ip[4];      // IP address
    uint8_t sn[4];      // Subnet mask
    uint8_t gw[4];      // Gateway
} wiz_NetInfo;

/**
 * @brief Initialize W5500 chip
 * @param hspi SPI handle for W5500 communication
 * @param htim Timer handle for periodic socket polling
 */
void W5500Init(SPI_HandleTypeDef *hspi, TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif
#endif
