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

#define MAX2(a, b)        ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c)     MAX2(MAX2(a, b), c)

#define CONTROL_BUS 1
#define SENSOR_BUS 2
#define TRACTIVE_BUS 3

#define CAN_BUS_CNT MAX3(CONTROL_BUS, SENSOR_BUS, TRACTIVE_BUS) + 1

void initCAN();
void drainFifoToQueue(FDCAN_HandleTypeDef *hfdcan);
int8_t sendCanFrame(uint16_t canId, uint8_t canBus, uint8_t *canData, uint8_t frameLen);

extern volatile uint32_t can_rx_packet_cnt[CAN_BUS_CNT];
extern volatile uint32_t can_rx_byte_cnt[CAN_BUS_CNT];
extern volatile uint32_t can_tx_packet_cnt[CAN_BUS_CNT];
extern volatile uint32_t can_tx_byte_cnt[CAN_BUS_CNT];

// BAUD rates for 3 CAN Buses (VERY IMPORTANT TO SET CORRECTLY!!!)
static const uint32_t canBaudRates[CAN_BUS_CNT] = {
    [CONTROL_BUS] = 1000000,  // 1 Mbps
    [SENSOR_BUS] = 1000000,  // 1 Mbps
    [TRACTIVE_BUS] = 125000,   // 500 kbps
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
