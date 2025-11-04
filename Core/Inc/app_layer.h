#ifndef __APP_LAYER__H__
#define __APP_LAYER__H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint32_t dropped_packets;

void initAppLayer();

#ifdef __cplusplus
}
#endif
#endif
