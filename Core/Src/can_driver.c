#include <can_driver.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "unix_time.h"
#include "forwarder_pb.pb.h"
#include "cmsis_os.h"
#include "app_layer.h"
#include "log_handler.h"

extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;
extern FDCAN_HandleTypeDef hfdcan3;
extern osMessageQueueId_t canSrcQueueHandle;

// Clock for 3 CAN peripherals
volatile uint64_t unixMicroseconds[3];

/**
 * @brief Drain all messages from FDCAN FIFO and put them in canSrcQueueHandle
 * @param hfdcan: FDCAN handle (hfdcan1, hfdcan2, or hfdcan3)
 * @retval None
 */
void drainFifoToQueue(FDCAN_HandleTypeDef *hfdcan) {
	FDCAN_RxHeaderTypeDef rxHeader;
	uint8_t rxData[8];
	CanFrame canFrame;
	uint32_t fifoFillLevel;

	// Get current FIFO fill level
	fifoFillLevel = HAL_FDCAN_GetRxFifoFillLevel(hfdcan, FDCAN_RX_FIFO0);

	// Determine which CAN bus this is
	uint32_t canBusId = getCANBusID(hfdcan);
	if (canBusId < 0) return;

	// Process all messages in FIFO
	while (fifoFillLevel > 0) {
		// Get message from FIFO
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, rxData) == HAL_OK) {
			// Convert to CanFrame structure
			canFrame.can_id = rxHeader.Identifier;
			canFrame.can_bus = canBusId;

			// Calculate absolute timestamp in microseconds
			canFrame.timestamp = (uint64_t)(unixMicroseconds[canBusId] + (uint64_t)rxHeader.RxTimestamp) * 1000U;

			// Copy data
			uint32_t dataLength = rxHeader.DataLength; // Extract DLC
			if (dataLength > 8) dataLength = 8; // Safety check

			memset(canFrame.can_data, 0, 8);
			memcpy(canFrame.can_data, rxData, dataLength);

			// Try to put message in queue
			if (osMessageQueuePut(canSrcQueueHandle, &canFrame, 0, 0) != osOK) {
				// TODO: Implement proper error handling for dropped messages
				dropped_packets++;
			}

			fifoFillLevel--;
		} else {
			log_msg(LL_ERR, "Failed to read message from CAN %d FIFO", canBusId);
			break;
		}
	}
}

void initCAN() {
	FDCAN_FilterTypeDef sFilterConfig;
  /* Configure RX filter to accept all messages */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x000;
  sFilterConfig.FilterID2 = 0x7FF;

  // Configure filters for all three FDCAN instances
  if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK) {
  	Error_Handler();
  }

  if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK) {
  	Error_Handler();
  }

  if (HAL_FDCAN_ConfigFilter(&hfdcan3, &sFilterConfig) != HAL_OK) {
  	Error_Handler();
  }

  // Set Wraparound interrupt to line 1
  if (HAL_FDCAN_ConfigInterruptLines(&hfdcan1, FDCAN_IT_TIMESTAMP_WRAPAROUND, FDCAN_INTERRUPT_LINE1) != HAL_OK) {
	  Error_Handler();
  }
  if (HAL_FDCAN_ConfigInterruptLines(&hfdcan2, FDCAN_IT_TIMESTAMP_WRAPAROUND, FDCAN_INTERRUPT_LINE1) != HAL_OK) {
	  Error_Handler();
  }
  if (HAL_FDCAN_ConfigInterruptLines(&hfdcan3, FDCAN_IT_TIMESTAMP_WRAPAROUND, FDCAN_INTERRUPT_LINE1) != HAL_OK) {
	  Error_Handler();
  }

  // Enable Timestamp Wraparound interrupt
  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_TIMESTAMP_WRAPAROUND, 0) != HAL_OK) {
  	Error_Handler();
  }
  if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_TIMESTAMP_WRAPAROUND, 0) != HAL_OK) {
  	Error_Handler();
  }
  if (HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_TIMESTAMP_WRAPAROUND, 0) != HAL_OK) {
  	Error_Handler();
  }

  // Set FIFO Watermark to 48/64
  if (HAL_FDCAN_ConfigFifoWatermark(&hfdcan1, FDCAN_CFG_RX_FIFO0, 48) != HAL_OK) {
  	Error_Handler();
  }
  if (HAL_FDCAN_ConfigFifoWatermark(&hfdcan2, FDCAN_CFG_RX_FIFO0, 48) != HAL_OK) {
  	Error_Handler();
  }
  if (HAL_FDCAN_ConfigFifoWatermark(&hfdcan3, FDCAN_CFG_RX_FIFO0, 48) != HAL_OK) {
  	Error_Handler();
  }

  // Activate FIFO Watermark interrupts on Line 0 (default)
  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_WATERMARK, 0) != HAL_OK) {
  	Error_Handler();
  }
  if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_WATERMARK, 0) != HAL_OK) {
  	Error_Handler();
  }
  if (HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_WATERMARK, 0) != HAL_OK) {
  	Error_Handler();
  }

  // Get current UNIX time in uS. We will now use CAN Timestamp Counters from now on
  unixMicroseconds[0] = getUnixTimeMicroseconds();
  unixMicroseconds[1] = getUnixTimeMicroseconds();
  unixMicroseconds[2] = getUnixTimeMicroseconds();

  // Enable Timestamp Counter
  if (HAL_FDCAN_EnableTimestampCounter(&hfdcan1, FDCAN_TIMESTAMP_INTERNAL) != HAL_OK) {
	  Error_Handler();
  }
  if (HAL_FDCAN_EnableTimestampCounter(&hfdcan2, FDCAN_TIMESTAMP_INTERNAL) != HAL_OK) {
	  Error_Handler();
  }
  if (HAL_FDCAN_EnableTimestampCounter(&hfdcan3, FDCAN_TIMESTAMP_INTERNAL) != HAL_OK) {
	  Error_Handler();
  }

  // Start Can Modules
  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
  	Error_Handler();
  }

  if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK) {
  	Error_Handler();
  }

  if (HAL_FDCAN_Start(&hfdcan3) != HAL_OK) {
  	Error_Handler();
  }
}

// Watermark Reached Interrupt
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	// Handle FIFO watermark - drain all messages to queue
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_WATERMARK) != RESET) {
		drainFifoToQueue(hfdcan);
	}
}

// Timestamp Wraparound Interrupt
void HAL_FDCAN_TimestampWraparoundCallback(FDCAN_HandleTypeDef *hfdcan) {
	// Rare bug could happen where CAN item is inserted before this interrupt is called, causing wrong timestamp
	drainFifoToQueue(hfdcan);

	uint32_t canBusId = getCANBusID(hfdcan);
	if (canBusId < 0) return;

	uint64_t wraparoundIncrement = 65536UL * (1000000UL / canBaudRates[canBusId]);
	unixMicroseconds[canBusId] += wraparoundIncrement;
}


// Send CanFrame to the appropriate CAN peripheral
int8_t sendCanFrame(uint16_t canId, uint8_t canBus, uint8_t *canData, uint8_t frameLen) {
	FDCAN_HandleTypeDef *hfdcan;
	FDCAN_TxHeaderTypeDef txHeader;
	
	// Get CAN peripheral from bus ID using inline function
	hfdcan = getCANPeripheralFromBusID(canBus);
	if (hfdcan == NULL) {
		return -1; // Invalid CAN bus ID
	}
	
	// Configure TX header
	txHeader.Identifier = canId;
	txHeader.IdType = FDCAN_STANDARD_ID;
	txHeader.TxFrameType = FDCAN_DATA_FRAME;
	txHeader.DataLength = frameLen;
	txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	txHeader.BitRateSwitch = FDCAN_BRS_OFF;
	txHeader.FDFormat = FDCAN_CLASSIC_CAN;
	txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	txHeader.MessageMarker = 0;

	if (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &txHeader, canData) != HAL_OK) {
		return -1; // Failed to add message to TX FIFO
	}
	
	return 0; // Success
}


