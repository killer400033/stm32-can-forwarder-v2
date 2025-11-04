#ifndef INC_W5500_DRIVER_H_
#define INC_W5500_DRIVER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DHCP_SOCKET     0
#define DNS_SOCKET      1
#define WS_SOCKET       2
#define NTP_SOCKET      3
#define MAX_HTTPSOCK    6

void W5500Init();
void sendData(uint8_t* pBuf, uint16_t len);

#ifdef __cplusplus
}
#endif
#endif
