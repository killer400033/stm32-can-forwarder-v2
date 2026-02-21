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

#define CONTROL_BUS 1
#define SENSOR_BUS 2
#define TRACTIVE_BUS 3

void initCAN();
void drainFifoToQueue(FDCAN_HandleTypeDef *hfdcan);
int8_t sendCanFrame(uint16_t canId, uint8_t canBus, uint8_t *canData, uint8_t frameLen);

// BAUD rates for 3 CAN Buses (VERY IMPORTANT TO SET CORRECTLY!!!)
static const uint32_t canBaudRates[3] = {
	1000000,  // CAN1: 1 Mbps
	1000000,  // CAN2: 1 Mbps
	500000    // CAN3: 500 kbps
};

// Inline function for getting CAN bus ID
static inline int8_t getCANBusID(FDCAN_HandleTypeDef *hfdcan) {
    if (hfdcan->Instance == FDCAN1) {
        return CONTROL_BUS;
    } else if (hfdcan->Instance == FDCAN2) {
        return SENSOR_BUS;
    } else if (hfdcan->Instance == FDCAN3) {
        return TRACTIVE_BUS;
    } else {
        return -1; // Unknown FDCAN instance
    }
}

// Inline function for getting CAN peripheral from bus ID
static inline FDCAN_HandleTypeDef* getCANPeripheralFromBusID(uint32_t canBus) {
    switch (canBus) {
        case CONTROL_BUS:
            return &hfdcan1;
        case SENSOR_BUS:
            return &hfdcan2;
        case TRACTIVE_BUS:
            return &hfdcan3;
        default:
            return NULL; // Invalid CAN bus ID
    }
}

#ifdef __cplusplus
}
#endif
#endif
