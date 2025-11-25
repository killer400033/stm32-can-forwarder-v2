#include "storage.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "log_handler.h"
#include "unix_time.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"

osThreadId_t storageTaskHandle;
const osThreadAttr_t storageTask_attributes = {
  .name = "Storage",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

extern osMessageQueueId_t storageCanQueueHandle;
extern osMessageQueueId_t storageLogQueueHandle;
extern SD_HandleTypeDef hsd1;

// Maximum number of CAN frames to write to file at once
#define MAX_CAN_FRAME_CNT 5
CanFrame canFrames[MAX_CAN_FRAME_CNT];
uint8_t canFrameCount = 0;

static char can_filename[64];
static char log_filename[64];

void storageThread(void *argument);
static void SD_Peripheral_Init(void);

static void SD_Peripheral_Init(void) {
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 40-1;
  if (HAL_SD_Init(&hsd1) != HAL_OK)
  {
  	log_msg(LL_ERR, "Failed to initialize SD peripheral");
  }
}

void initStorage() {
	// Generate filenames based on current date/time
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	getCurrentDateTime(&sTime, &sDate);
	
	// Format: YYYY-MM-DD_HH-MM-SS_can.bin and YYYY-MM-DD_HH-MM-SS_log.txt
	snprintf(can_filename, sizeof(can_filename), "%04d-%02d-%02d_%02d-%02d-%02d_can.bin",
			2000 + sDate.Year, sDate.Month, sDate.Date,
			sTime.Hours, sTime.Minutes, sTime.Seconds);
			
	snprintf(log_filename, sizeof(log_filename), "%04d-%02d-%02d_%02d-%02d-%02d_log.txt",
			2000 + sDate.Year, sDate.Month, sDate.Date,
			sTime.Hours, sTime.Minutes, sTime.Seconds);
			
	log_msg(LL_DBG, "CAN file: %s", can_filename);
	log_msg(LL_DBG, "Log file: %s", log_filename);
	storageTaskHandle = osThreadNew(storageThread, NULL, &storageTask_attributes);
	// TODO: Wipe SD card if its too full
}

void storageThread(void *argument) {
	uint32_t prev_time = osKernelGetTickCount();
	uint32_t last_sync_time = osKernelGetTickCount();
	uint32_t can_bytes_written = 0;
	uint32_t log_bytes_written = 0;
	
	FIL canFile, logFile;
	log_queue_entry_t logEntry;

	for (;;) {
		// Wait for SD card insertion
		while (HAL_GPIO_ReadPin(SDMMC_CARD_DETECT_GPIO_Port, SDMMC_CARD_DETECT_Pin)) {
			log_msg(LL_WRN, "No SD Card Detected...");
			osDelay(10000);
		}
		
		SD_Peripheral_Init();

		// Mount filesystem
		FRESULT res = f_mount(&SDFatFS, SDPath, 1);
		if (res == FR_OK) {
			log_msg(LL_DBG, "SD Card Mounted Successfully!");
			
			// Open both files
			FRESULT can_res = f_open(&canFile, can_filename, FA_OPEN_APPEND | FA_WRITE);
			FRESULT log_res = f_open(&logFile, log_filename, FA_OPEN_APPEND | FA_WRITE);

			while (can_res == FR_OK && log_res == FR_OK) {
				UINT bytes_written = 0;
				
				// === Process CAN data ===
				while (osMessageQueueGetCount(storageCanQueueHandle) > 0 && canFrameCount < MAX_CAN_FRAME_CNT) {
					osMessageQueueGet(storageCanQueueHandle, &(canFrames[canFrameCount++]), 0, 0);
				}
				
				if (canFrameCount > 0) {
					can_res = f_write(&canFile, canFrames, sizeof(CanFrame) * canFrameCount, &bytes_written);
					
					if (can_res != FR_OK) {
						log_msg(LL_ERR, "CAN file write error: %d", can_res);
					} else {
						can_bytes_written += bytes_written;
					}
					
					canFrameCount = 0;
				}
				
				// === Process log data ===
				while (osMessageQueueGetCount(storageLogQueueHandle) > 0) {
					if (osMessageQueueGet(storageLogQueueHandle, &logEntry, 0, 0) == osOK) {						
						log_res = f_write(&logFile, logEntry.start_ptr, logEntry.length, &bytes_written);
						
						if (log_res != FR_OK) {
							log_msg(LL_ERR, "Log file write error: %d", log_res);
							break;
						} else {
							log_bytes_written += bytes_written;
						}
					}
				}
				
				// Sync files every 5 seconds to update directory entry
				if (osKernelGetTickCount() - last_sync_time > 5000) {
					f_sync(&canFile);
					f_sync(&logFile);
					last_sync_time = osKernelGetTickCount();
				}
				
				// Throughput measurement every 10 seconds
				if (osKernelGetTickCount() - prev_time > 10000) {
					uint32_t speed = (log_bytes_written + can_bytes_written) / 10;
					prev_time = osKernelGetTickCount();
					log_msg(LL_DBG, "SD Throughput: %lu B/s (CAN: %lu B/s, Log: %lu B/s)", speed, can_bytes_written / 10, log_bytes_written / 10);
					can_bytes_written = 0;
					log_bytes_written = 0;
				}
				
				osThreadYield();
			}
				
			// Close both files
			f_close(&canFile);
			f_close(&logFile);
		}
		
		log_msg(LL_ERR, "SD Card Session Ended. Error: %d", res);
		// Cleanup and reinitialize
		f_mount(NULL, SDPath, 0);
		HAL_SD_DeInit(&hsd1);
		
		osDelay(1000);
		log_msg(LL_DBG, "Retrying SD Card Mount...");
	}
}
