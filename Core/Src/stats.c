#include "stats.h"
#include "cmsis_os.h"
#include "main.h"
#include "log_handler.h"
#include "w5500_driver.h"

// External variable declarations
extern volatile uint32_t dropped_packets;
extern volatile uint32_t can_send_errors;
extern volatile uint32_t can_read_errors;
extern uint32_t enqueueFailsInISR;

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
void statsThread(void *argument) {
    for (;;) {
        // Print statistics
        log_msg(LL_DBG, "Stats - Dropped packets: %lu, CAN send errors: %lu, CAN read errors: %lu", 
                dropped_packets, can_send_errors, can_read_errors);
        
        log_msg(LL_DBG, "Unix Time: %d", getUnixTimeSeconds());
        // Print enqueue fails only if greater than 0
        if (enqueueFailsInISR > 0) {
            log_msg(LL_ERR, "Stats - Enqueue fails in ISR: %lu", enqueueFailsInISR);
        }
        
        // Wait 10 seconds before next update
        osDelay(10000);
    }
}

