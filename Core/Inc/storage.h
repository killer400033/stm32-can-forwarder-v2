#ifndef INC_STORAGE_H_
#define INC_STORAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "forwarder_pb.pb.h"

#define MAX_CAN_FRAME_CNT 5

typedef struct CanFrameList {
    CanFrame canFrames[MAX_CAN_FRAME_CNT];
    uint8_t count;
} CanFrameList_t;

void initStorage();

#ifdef __cplusplus
}
#endif
#endif
