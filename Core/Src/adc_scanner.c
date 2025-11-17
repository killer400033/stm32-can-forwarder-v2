#include "adc_scanner.h"
#include "main.h"
#include <string.h>
#include <stdbool.h>
#include "unix_time.h"
#include "cmsis_os.h"
#include "forwarder_pb.pb.h"
#include "can_driver.h"
#include "dbc.h"
#include "log_handler.h"
#include "app_layer.h"

// DMA buffers for ADC conversions (uint16_t for 12-bit ADC)
static uint16_t adc1_dma_buffer[5];  // 5 channels for ADC1
static uint16_t adc2_dma_buffer[5];  // 5 channels for ADC2
// ADC3 uses BDMA which MUST be in SRAM4 (D3 domain) - BDMA cannot access other RAM!
static uint16_t adc3_dma_buffer[2] __attribute__((section(".sram4")));  // 2 channels for ADC3

// ADC completion tracking
static bool adc1_complete = false;
static bool adc2_complete = false;
static bool adc3_complete = false;

TIM_HandleTypeDef *ADCTimerInstance = NULL;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern osMessageQueueId_t canSrcQueueHandle;

static void pushToWSandCAN(CanFrame *frame, uint8_t length);

void ADC_Scanner_Init(TIM_HandleTypeDef *htim)
{
  // Calibrate ADC
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc2, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);

  ADCTimerInstance = htim;

  htim->Instance->PSC = 55000-1;
  htim->Instance->ARR = (ADC_TIM_CLK / 55000) / ADC_SAMPLE_RATE - 1;
  htim->Instance->CNT = 0;
  
  // Generate update event to load the prescaler value
  htim->Instance->EGR = TIM_EGR_UG;
  
  // Start timer with update (overflow) interrupt
  HAL_TIM_Base_Start_IT(htim);
}

void ADC_Scanner_TriggerConversions(void)
{
  // Reset completion tracking
  adc1_complete = false;
  adc2_complete = false;
  adc3_complete = false;

  // Trigger ADC conversions on all 3 ADCs simultaneously using DMA
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1_dma_buffer, 5);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc2_dma_buffer, 5);
  HAL_ADC_Start_DMA(&hadc3, (uint32_t*)adc3_dma_buffer, 2);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	// Mark the specific ADC as complete
	if (hadc->Instance == ADC1) {
		adc1_complete = true;
	} else if (hadc->Instance == ADC2) {
		adc2_complete = true;
	} else if (hadc->Instance == ADC3) {
		adc3_complete = true;
	}

	if (!adc1_complete || !adc2_complete || !adc3_complete) return;

	// Reusable CAN frame for all messages
	CanFrame frame = {0};
	frame.can_bus = CONTROL_BUS;
	frame.timestamp = getUnixTimeNanoseconds();
	float sensor_voltage;

	// Create CAN frame for coolant and oil temperature sensors (CAN ID 910)
	CANHUB_TEMP_SENSORS_t tempMsg = {0};
	sensor_voltage = ((float)adc1_dma_buffer[2] * 3.3f) / 4095.0f;
	tempMsg.OilTempSensorLeft = -0.0258f * sensor_voltage + 2.8204f;

	sensor_voltage = ((float)adc1_dma_buffer[3] * 3.3f) / 4095.0f;
	tempMsg.OilTempSensorRight = -0.0258f * sensor_voltage + 2.8204f;

	sensor_voltage = ((float)adc1_dma_buffer[4] * 3.3f) / 4095.0f;
	tempMsg.CoolingTempSensorLeft = -0.0262f * sensor_voltage + 2.8474f;

	sensor_voltage = ((float)adc2_dma_buffer[2] * 3.3f) / 4095.0f;
	tempMsg.CoolingTempSensorRight = -0.0262f * sensor_voltage + 2.8474f;

	if (Pack_CANHUB_TEMP_SENSORS(&tempMsg, frame.can_data, 8) == STATUS_OK) {
		frame.can_id = 910;
		pushToWSandCAN(&frame, 8);
	}

	// Create CAN frame for potentiometers (CAN ID 911)
	CANHUB_POTS_t potMsg = {0};
	potMsg.CanhubPot1 = ((float)adc2_dma_buffer[3] * 3.3f) / 4095.0f;
	potMsg.CanhubPot2 = ((float)adc2_dma_buffer[4] * 3.3f) / 4095.0f;

	if (Pack_CANHUB_POTS(&potMsg, frame.can_data, 8) == STATUS_OK) {
		frame.can_id = 911;
		pushToWSandCAN(&frame, 4);
	}

	// Create CAN frame for strain gauge linkages (CAN ID 913)
	CANHUB_STRAIN_LINKS_t strainLinkMsg = {0};
	strainLinkMsg.LinkStrain1 = (int16_t)(((float)adc1_dma_buffer[0] * 3.3f) / 4095.0f);
	strainLinkMsg.LinkStrain2 = (int16_t)(((float)adc1_dma_buffer[1] * 3.3f) / 4095.0f);
	strainLinkMsg.LinkStrain3 = (int16_t)(((float)adc2_dma_buffer[0] * 3.3f) / 4095.0f);
	strainLinkMsg.LinkStrain4 = (int16_t)(((float)adc2_dma_buffer[1] * 3.3f) / 4095.0f);

	if (Pack_CANHUB_STRAIN_LINKS(&strainLinkMsg, frame.can_data, 8) == STATUS_OK) {
		frame.can_id = 913;
		pushToWSandCAN(&frame, 8);
	}

	// Create CAN frame for strain gauge steering column (CAN ID 914)
	CANHUB_STRAIN_STEERING_t strainSteerMsg = {0};
	strainSteerMsg.SteeringStrain1 = (int16_t)(((float)adc3_dma_buffer[0] * 3.3f) / 4095.0f);
	strainSteerMsg.SteeringStrain2 = (int16_t)(((float)adc3_dma_buffer[1] * 3.3f) / 4095.0f);

	if (Pack_CANHUB_STRAIN_STEERING(&strainSteerMsg, frame.can_data, 8) == STATUS_OK) {
		frame.can_id = 914;
		pushToWSandCAN(&frame, 4);
	}

  // Stop DMA conversions
  HAL_ADC_Stop_DMA(&hadc1);
  HAL_ADC_Stop_DMA(&hadc2);
  HAL_ADC_Stop_DMA(&hadc3);
}

static void pushToWSandCAN(CanFrame *frame, uint8_t length)
{
  if (osMessageQueueGetSpace(canSrcQueueHandle) > 0) {
    osMessageQueuePut(canSrcQueueHandle, frame, 0, 0);
  }
  else {
    // TODO: Implement error handling for dropped messages
    dropped_packets++;
  }
  if (sendCanFrame(frame->can_id, frame->can_bus, frame->can_data, length) != 0) {
   log_msg(LL_ERR, "Failed to send CAN ID %d through bus %d", frame->can_id, frame->can_bus);
  }
}

