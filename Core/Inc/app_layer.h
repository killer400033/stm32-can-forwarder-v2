#ifndef __APP_LAYER__H__
#define __APP_LAYER__H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// DNS configuration
#define WS_DOMAIN_NAME "ws-alb-1889248813.ap-southeast-2.elb.amazonaws.com"
#define WS_DOMAIN_PATH "/api/decoder/connection"

extern volatile uint32_t dropped_packets;

void initAppLayer();

#ifdef __cplusplus
}
#endif
#endif
