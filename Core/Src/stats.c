#include "stats.h"
#include "cmsis_os.h"
#include "main.h"
#include "log_handler.h"
#include "w5500_driver.h"
#include "can_driver.h"

// External variable declarations
extern volatile uint32_t dropped_packets;
extern volatile uint32_t can_send_errors;
extern volatile uint32_t can_read_errors;
extern volatile uint32_t enqueueFailsInISR;
extern volatile uint32_t spiErrCount;

// Thread handle and attributes
osThreadId_t statsTaskHandle;
const osThreadAttr_t statsTask_attributes = {
  .name = "Stats",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

// Thread function
void statsThread(void *argument);

/**
 * @brief Initialize statistics monitoring
 */
void initStats(void) {
    statsTaskHandle = osThreadNew(statsThread, NULL, &statsTask_attributes);
}

/**
 * @brief Statistics monitoring thread
 * Prints system statistics every 10 seconds
 */
static void logBusStats(uint8_t bus, FDCAN_HandleTypeDef *hfdcan, const char *busName) {
    FDCAN_ErrorCountersTypeDef errCnt;
    HAL_FDCAN_GetErrorCounters(hfdcan, &errCnt);

    uint32_t rx_pkt   = can_rx_packet_cnt[bus];
    uint32_t rx_bytes = can_rx_byte_cnt[bus];
    uint32_t tx_pkt   = can_tx_packet_cnt[bus];
    uint32_t tx_bytes = can_tx_byte_cnt[bus];
    can_rx_packet_cnt[bus] = 0;
    can_rx_byte_cnt[bus]   = 0;
    can_tx_packet_cnt[bus] = 0;
    can_tx_byte_cnt[bus]   = 0;

    // Standard CAN frame overhead = 47 bits, data = DLC*8 bits, stuffing ~+20%
    uint32_t bits = ((rx_pkt + tx_pkt) * 47 + (rx_bytes + tx_bytes) * 8) * 6 / 5;
    uint32_t usage_pct = bits * 100 / (canBaudRates[bus] * 10);
    if (usage_pct > 100) usage_pct = 100;

    char bar[21];
    uint32_t filled = usage_pct * 20 / 100;
    memset(bar, '#', filled);
    memset(bar + filled, '-', 20 - filled);
    bar[20] = '\0';

    log_msg(LL_DBG, "%s: RxErr=%lu, TxErr=%lu", busName, (uint32_t)errCnt.RxErrorCnt, (uint32_t)errCnt.TxErrorCnt);
    log_msg(LL_DBG, "%s: [%s] %lu%%", busName, bar, usage_pct);
}

void statsThread(void *argument) {
    for (;;) {
				// Wait 10 seconds before next update
				osDelay(10000);

        // Print statistics
        log_msg(LL_DBG, "Dropped Packets: %lu, CAN Send Errs: %lu, CAN Read Errs: %lu, SPI Send Errs: %lu",
                dropped_packets, can_send_errors, can_read_errors, spiErrCount);
        dropped_packets = 0;

        // Print enqueue fails only if greater than 0
        if (enqueueFailsInISR > 0) {
            log_msg(LL_ERR, "Enqueue fails in ISR: %lu", enqueueFailsInISR);
        }

        logBusStats(CONTROL_BUS,  &hfdcan1, "Control");
        logBusStats(SENSOR_BUS,   &hfdcan2, "Sensor");
        logBusStats(TRACTIVE_BUS, &hfdcan3, "Tractive");
    }
}
