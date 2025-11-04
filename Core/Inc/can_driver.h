#ifndef __CAN_DRIVER__H__
#define __CAN_DRIVER__H__

#include "stm32h7xx_hal.h"
#include <stdint.h>
#include "forwarder_pb.pb.h"

// External CAN handle declarations
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;
extern FDCAN_HandleTypeDef hfdcan3;

#ifdef __cplusplus
extern "C" {
#endif

void initCAN();
void drainFifoToQueue(FDCAN_HandleTypeDef *hfdcan);
int8_t sendCanFrame(uint16_t canId, uint8_t canBus, uint8_t *canData, uint8_t frameLen);

// BAUD rates for 3 CAN Buses (VERY IMPORTANT TO SET CORRECTLY!!!)
static const uint32_t canBaudRates[3] = {
	1000000,  // CAN1: 1 Mbps
	1000000,  // CAN2: 1 Mbps
	250000    // CAN3: 250 kbps
};

// Inline function for getting CAN bus ID
static inline int8_t getCANBusID(FDCAN_HandleTypeDef *hfdcan) {
    if (hfdcan->Instance == FDCAN1) {
        return 0;
    } else if (hfdcan->Instance == FDCAN2) {
        return 1;
    } else if (hfdcan->Instance == FDCAN3) {
        return 2;
    } else {
        return -1; // Unknown FDCAN instance
    }
}

// Inline function for getting CAN peripheral from bus ID
static inline FDCAN_HandleTypeDef* getCANPeripheralFromBusID(uint32_t canBus) {
    switch (canBus) {
        case 0:
            return &hfdcan1;
        case 1:
            return &hfdcan2;
        case 2:
            return &hfdcan3;
        default:
            return NULL; // Invalid CAN bus ID
    }
}

#ifdef __cplusplus
}
#endif
#endif
