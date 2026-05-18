#ifndef __STREAM__H__
#define __STREAM__H__

#include <stdint.h>
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

// DNS configuration
#define WS_DOMAIN_NAME "ws-alb-1889248813.ap-southeast-2.elb.amazonaws.com"
#define WS_DOMAIN_PATH "/api/decoder/connection"
#define WS_DOMAIN_PORT 80

void initStream(RNG_HandleTypeDef *hrng);

#ifdef __cplusplus
}
#endif
#endif
