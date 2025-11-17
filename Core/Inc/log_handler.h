#ifndef INC_LOG_HANDLER_H_
#define INC_LOG_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdarg.h>
#include "stm32h7xx_hal.h"
#include "cmsis_os.h"

// Log levels
typedef enum {
    LL_DBG = 0,  // Debug
    LL_WRN = 1,  // Warning
    LL_ERR = 2   // Error
} log_level_t;

// Configuration
#define LOG_BUFFER_SIZE     (2048)   // Total buffer size in bytes
#define LOG_MAX_MSG_LENGTH  (256)    // Maximum length of a single log message
#define MIN_MSG_IN_BUFFER   (LOG_BUFFER_SIZE / LOG_MAX_MSG_LENGTH)     // Minimum possible number of messages in buffer
#define LOG_USE_COLORS      1        // Set to 1 to enable ANSI colors, 0 to disable

// Log queue entry structure
typedef struct {
    const char* start_ptr;  // Start position in log_buffer
    uint16_t length;       // Length of the log message
} log_queue_entry_t;

// Initialize the logging system
// Pass your UART handle for DMA transmission
void log_init(UART_HandleTypeDef* huart);

// Main logging function with printf-style formatting
// Automatically sends via DMA over UART or queues if busy
void log_msg(log_level_t level, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif /* INC_LOG_HANDLER_H_ */

