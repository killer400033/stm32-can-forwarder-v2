#include "storage.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "log_handler.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

osThreadId_t storageTaskHandle;
const osThreadAttr_t storageTask_attributes = {
  .name = "Storage",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

extern osMessageQueueId_t storageCanQueueHandle;
extern SD_HandleTypeDef hsd1;

void storageThread(void *argument);
static void SD_Init(void);

static void SD_Init(void) {
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 0;
  if (HAL_SD_Init(&hsd1) != HAL_OK)
  {
  	log_msg(LL_ERR, "Failed to initialize SD peripheral");
  }
}

void initStorage() {
	storageTaskHandle = osThreadNew(storageThread, NULL, &storageTask_attributes);
}

void storageThread(void *argument) {
	uint32_t prev_time = osKernelGetTickCount();
	uint32_t prev_cnt = 0;
	CanFrameList_t canFrameList = {0};
	canFrameList.count = 0;
	uint32_t cnt = 0;

	for (;;) {
		while (HAL_GPIO_ReadPin(SDMMC_CARD_DETECT_GPIO_Port, SDMMC_CARD_DETECT_Pin)) {
			log_msg(LL_WRN, "No SD Card Detected...");
			osDelay(1000);
		}
		SD_Init();

		osDelay(1000);


		FRESULT res = f_mount(&SDFatFS, SDPath, 1);
		if (res == FR_OK) {
			log_msg(LL_DBG, "SD Card Mounted Successfully!!!");
			res = f_open(&SDFile, "05-02-25 CAN 1.can", FA_OPEN_ALWAYS | FA_WRITE);

			while (res == FR_OK) {
				// Buffer MAX_CAN_FRAME_CNT Can Frames before writing to file
				while (osMessageQueueGetCount(storageCanQueueHandle) > 0 && canFrameList.count < MAX_CAN_FRAME_CNT) {
					osMessageQueueGet(storageCanQueueHandle, &(canFrameList.canFrames[canFrameList.count++]), 0, 0);
				}

				// Write data to SD card
				res = f_lseek(&SDFile, sizeof(CanFrame)*cnt);
				UINT i;
				res = f_write(&SDFile, canFrameList.canFrames, sizeof(CanFrame) * canFrameList.count, &i);

				// Flush every 1000 can frames
				if (cnt % 1000 == 0) {
					f_sync(&SDFile);
				}
				cnt += canFrameList.count;
				canFrameList.count = 0;

				// Throughput measuring
				if (osKernelGetTickCount() - prev_time > 10000) {
					uint32_t speed = (cnt - prev_cnt) * sizeof(CanFrame) / 10;
					prev_time = osKernelGetTickCount();
					prev_cnt = cnt;
					log_msg(LL_DBG, "SD Card: %ld B/s", speed);
				}
				osThreadYield();
			}
			f_close(&SDFile);
		}

		// If mount failed, check if SD is plugged in and restart SD Driver
		MX_FATFS_DeInit();
		log_msg(LL_ERR, "Failed To Mount SD Card. Error Code %d", res);

		osDelay(1000);
		log_msg(LL_DBG, "Retrying SD Card Mount...");
		MX_FATFS_Init();
	}
}
