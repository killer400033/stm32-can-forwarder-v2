#include "log_handler.h"
#include "unix_time.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>

osThreadId_t logTaskHandle;
const osThreadAttr_t logTask_attributes = {
  .name = "Log Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

// Log buffer
char log_buffer[LOG_BUFFER_SIZE];
static volatile uint32_t log_write_pos = 0;

// UART handle for DMA transmission
static UART_HandleTypeDef* uart_handle = NULL;

// Queue for pending log transmissions
extern osMessageQueueId_t logQueueHandle;
extern osMessageQueueId_t storageLogQueueHandle;

// Mutex for thread-safe operations
static osMutexId_t log_mutex = NULL;

static void log_task(void *argument);
static void enqueueBufferedLog(log_queue_entry_t entry, osMessageQueueId_t queueHandle);

// ANSI color codes
#if LOG_USE_COLORS
    #define ANSI_COLOR_GREEN   "\x1b[32m"
    #define ANSI_COLOR_YELLOW  "\x1b[33m"
    #define ANSI_COLOR_RED     "\x1b[31m"
    #define ANSI_COLOR_RESET   "\x1b[0m"
#else
    #define ANSI_COLOR_GREEN   ""
    #define ANSI_COLOR_YELLOW  ""
    #define ANSI_COLOR_RED     ""
    #define ANSI_COLOR_RESET   ""
#endif

// Helper function to get log level string with color
static const char* log_level_color(log_level_t level) {
    switch (level) {
        case LL_DBG: return ANSI_COLOR_GREEN;
        case LL_WRN: return ANSI_COLOR_YELLOW;
        case LL_ERR: return ANSI_COLOR_RED;
        default: return "";
    }
}

// Helper function to get log level string
static const char* log_level_to_string(log_level_t level) {
    switch (level) {
        case LL_DBG: return "DBG";
        case LL_WRN: return "WRN";
        case LL_ERR: return "ERR";
        default: return "UNK";
    }
}

// Initialize the logging system
void log_init(UART_HandleTypeDef* huart) {
    memset(log_buffer, 0, LOG_BUFFER_SIZE);
    log_write_pos = 0;
    uart_handle = huart;
    
    // Create mutex if FreeRTOS is running
    if (log_mutex == NULL) {
        const osMutexAttr_t mutex_attr = {
            .name = "LogMutex",
            .attr_bits = osMutexRecursive,
            .cb_mem = NULL,
            .cb_size = 0U
        };
        log_mutex = osMutexNew(&mutex_attr);
    }
    logTaskHandle = osThreadNew(log_task, NULL, &logTask_attributes);
    log_msg(LL_DBG, "Log handler initialized");
}

// Main logging function
void log_msg(log_level_t level, const char* format, ...) {
    // Thread-safe write to buffer
    if (log_mutex != NULL) {
        osMutexAcquire(log_mutex, osWaitForever);
    }
    
    // Calculate available linear space from current write position
    uint32_t linear_space = LOG_BUFFER_SIZE - log_write_pos;
    
    // Need enough space for full message
    if (linear_space < LOG_MAX_MSG_LENGTH) {
        // Not enough linear space at end, wrap to beginning
        log_write_pos = 0;
        linear_space = LOG_BUFFER_SIZE;
    }
    
    // Get current RTC time and date
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    getCurrentDateTime(&sTime, &sDate);
    
    // Format header directly into buffer: [YYYY-MM-DD HH:MM:SS][LEVEL] 
    int header_len = snprintf(&log_buffer[log_write_pos], linear_space,
                              "[%04d-%02d-%02d %02d:%02d:%02d][%s%s%s] ",
                              2000 + sDate.Year,
                              sDate.Month,
                              sDate.Date,
                              sTime.Hours,
                              sTime.Minutes,
                              sTime.Seconds,
                              log_level_color(level),
                              log_level_to_string(level),
                              ANSI_COLOR_RESET);
    
    if (header_len > 0 && header_len < (int)linear_space - 2) {
        // Add the user message
        va_list args;
        va_start(args, format);
        int msg_len = vsnprintf(&log_buffer[log_write_pos + header_len], 
                                linear_space - header_len - 2,  // Leave room for \r\n
                                format, args);
        va_end(args);
        
        if (msg_len > 0) {
            int total_len = header_len + msg_len;
            if (total_len < (int)(linear_space - 2)) {
                // Get pointer to start of message for DMA
                const char* msg_start = &log_buffer[log_write_pos];
                
                // Update write position and add line ending
                log_write_pos += total_len;
                log_buffer[log_write_pos++] = '\r';
                log_buffer[log_write_pos++] = '\n';
                total_len += 2;

                log_queue_entry_t entry;
                entry.start_ptr = msg_start;
                entry.length = total_len;

                // Queue log message to SD card storage
                enqueueBufferedLog(entry, storageLogQueueHandle);
                
                // Try to send via DMA if UART is ready
                if (uart_handle != NULL) {
                    HAL_UART_StateTypeDef state = HAL_UART_GetState(uart_handle);
                    
                    if (state == HAL_UART_STATE_READY) {
                        // UART is free, send immediately
                        HAL_UART_Transmit_DMA(uart_handle, (uint8_t*)msg_start, total_len);
                    } else if (logQueueHandle != NULL) {
                        // UART is busy, queue the message
                        osMessageQueuePut(logQueueHandle, &entry, 0, 0);
                    }
                }
            }
        }
    }
    
    if (log_mutex != NULL) {
        osMutexRelease(log_mutex);
    }
}

/**
 * @brief Enqueue a log message to a buffered queue for storage or websocket transmission
 * @param entry The log message entry
 * @param queueHandle The queue handle
 */
static void enqueueBufferedLog(log_queue_entry_t entry, osMessageQueueId_t queueHandle) {
    if (osMessageQueueGetSpace(queueHandle) > 0 && osMessageQueueGetCount(queueHandle) < MIN_MSG_IN_BUFFER) {
        // Queue has space and is not full, enqueue the message
        osMessageQueuePut(queueHandle, &entry, 0, 0);
    }
    else {
        // Better guarantee of not accidentally sending older log entry that has been overwritten in buffer
        log_queue_entry_t throwaway_entry;
        osMessageQueueGet(queueHandle, &throwaway_entry, NULL, 0);
        osMessageQueuePut(queueHandle, &entry, 0, 0);
    }
}

/**
 * @brief Log transmission task - processes queued log messages
 * @param argument Unused
 * @note This task blocks on the log queue and sends messages via DMA when UART is ready
 */
void log_task(void *argument)
{
    log_queue_entry_t entry;
    
    for(;;)
    {
        // Block waiting for a queued log message
        if (osMessageQueueGet(logQueueHandle, &entry, NULL, osWaitForever) == osOK) {
            // Wait until UART is ready (blocking)
            while (uart_handle != NULL && HAL_UART_GetState(uart_handle) != HAL_UART_STATE_READY) {
                osDelay(1);
            }
            
            // UART is ready, send the queued message
            if (uart_handle != NULL) {
                HAL_UART_Transmit_DMA(uart_handle, (uint8_t*)entry.start_ptr, entry.length);
            }
        }
    }
}

