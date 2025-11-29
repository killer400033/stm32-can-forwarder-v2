#ifndef __STREAM__H__
#define __STREAM__H__

#include <stdint.h>
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

// UDP streaming port
#define STREAM_UDP_PORT 9010
#define STREAM_DOMAIN_NAME "ws-alb-1889248813.ap-southeast-2.elb.amazonaws.com"

void initStream(void);

#ifdef __cplusplus
}
#endif
#endif

