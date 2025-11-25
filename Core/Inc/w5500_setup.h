#ifndef INC_W5500_SETUP_H_
#define INC_W5500_SETUP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

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
 */
void W5500Init(void);

#ifdef __cplusplus
}
#endif
#endif
