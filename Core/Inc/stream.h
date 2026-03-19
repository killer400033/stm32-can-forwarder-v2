#ifndef __STREAM__H__
#define __STREAM__H__

#include <stdint.h>
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

// UDP streaming port
#define STREAM_UDP_PORT 9010
#define STREAM_DOMAIN_NAME "can-decoder-nlb-d6e50c7aacb346c5.elb.ap-southeast-2.amazonaws.com"

void initStream(void);

#ifdef __cplusplus
}
#endif
#endif

