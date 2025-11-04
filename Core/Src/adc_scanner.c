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

// ADC completion tracking
static bool adc1_complete = false;
static bool adc2_complete = false;
static bool adc3_complete = false;

// DMA buffers for ADC conversions (uint16_t for 12-bit ADC)
static uint16_t adc1_dma_buffer[5];  // 5 channels for ADC1
static uint16_t adc2_dma_buffer[5];  // 5 channels for ADC2
// ADC3 uses BDMA which MUST be in SRAM4 (D3 domain) - BDMA cannot access other RAM!
static uint16_t adc3_dma_buffer[2] __attribute__((section(".sram4")));  // 2 channels for ADC3

TIM_HandleTypeDef *ADCTimerInstance = NULL;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern osMessageQueueId_t adcQueueHandle;
extern osMessageQueueId_t wsCanQueueHandle;

// ADC Packet Task
osThreadId_t adcPacketTaskHandle;
const osThreadAttr_t adcPacketTask_attributes = {
  .name = "ADC Packet",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void adcPacketThread(void *argument);
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

  // Create ADC packet task
  adcPacketTaskHandle = osThreadNew(adcPacketThread, NULL, &adcPacketTask_attributes);
}

void ADC_Scanner_TriggerConversions(void)
{
  /* USER CODE BEGIN ADC_Scanner_TriggerConversions */
  // Reset completion tracking
  adc1_complete = false;
  adc2_complete = false;
  adc3_complete = false;
  
  // Trigger ADC conversions on all 3 ADCs simultaneously using DMA
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1_dma_buffer, 5);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc2_dma_buffer, 5);
  HAL_ADC_Start_DMA(&hadc3, (uint32_t*)adc3_dma_buffer, 2);
  /* USER CODE END ADC_Scanner_TriggerConversions */
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

  // Only proceed when all 3 ADCs have completed
  if (adc1_complete && adc2_complete && adc3_complete) {
    // All ADCs completed - now read the data from DMA buffers
  	ADC_ScanData_t data;
    data.timestamp = getUnixTimeMicroseconds();

    // ADC1 DMA buffer[0] - Channel 16 - Strain gauge 0
    data.strain_voltages[0] = ((float)adc1_dma_buffer[0] * 3.3f) / 4095.0f;
    
    // ADC1 DMA buffer[1] - Channel 17 - Strain gauge 1
    data.strain_voltages[1] = ((float)adc1_dma_buffer[1] * 3.3f) / 4095.0f;

    // ADC1 DMA buffer[2] - Channel 18 - Gearbox temperature sensor 0
    float sensor_voltage = ((float)adc1_dma_buffer[2] * 3.3f) / 4095.0f;
    data.gearbox_temp[0] = -0.0258f * sensor_voltage + 2.8204f;

    // ADC1 DMA buffer[3] - Channel 19 - Gearbox temperature sensor 1
    sensor_voltage = ((float)adc1_dma_buffer[3] * 3.3f) / 4095.0f;
    data.gearbox_temp[1] = -0.0258f * sensor_voltage + 2.8204f;

    // ADC1 DMA buffer[4] - Channel 3 - Coolant temperature sensor 0
    sensor_voltage = ((float)adc1_dma_buffer[4] * 3.3f) / 4095.0f;
    data.coolant_temp[0] = -0.0262f * sensor_voltage + 2.8474f;

    // ADC2 DMA buffer[0] - Channel 14 - Strain gauge 2
    data.strain_voltages[2] = ((float)adc2_dma_buffer[0] * 3.3f) / 4095.0f;
    
    // ADC2 DMA buffer[1] - Channel 15 - Strain gauge 3
    data.strain_voltages[3] = ((float)adc2_dma_buffer[1] * 3.3f) / 4095.0f;

    // ADC2 DMA buffer[2] - Channel 7 - Coolant temperature sensor 1
    sensor_voltage = ((float)adc2_dma_buffer[2] * 3.3f) / 4095.0f;
    data.coolant_temp[1] = -0.0262f * sensor_voltage + 2.8474f;

    // ADC2 DMA buffer[3] - Channel 4 - Potentiometer 0
    data.pot_voltages[0] = ((float)adc2_dma_buffer[3] * 3.3f) / 4095.0f;
    
    // ADC2 DMA buffer[4] - Channel 8 - Potentiometer 1
    data.pot_voltages[1] = ((float)adc2_dma_buffer[4] * 3.3f) / 4095.0f;

    // ADC3 DMA buffer[0] - Channel 0 - Strain gauge 4
    data.strain_voltages[4] = ((float)adc3_dma_buffer[0] * 3.3f) / 4095.0f;
    
    // ADC3 DMA buffer[1] - Channel 1 - Strain gauge 5
    data.strain_voltages[5] = ((float)adc3_dma_buffer[1] * 3.3f) / 4095.0f;
    
    // Append ADC data to queue
    if (osMessageQueueGetSpace(adcQueueHandle) > 0) {
      osMessageQueuePut(adcQueueHandle, &data, 0, 0);
    }
    else {
        log_msg(LL_ERR, "ADC queue is full");
    }
    
    // Stop DMA conversions
    HAL_ADC_Stop_DMA(&hadc1);
    HAL_ADC_Stop_DMA(&hadc2);
    HAL_ADC_Stop_DMA(&hadc3);
  }
}

// ADC Packet Task Thread
void adcPacketThread(void *argument)
{
	ADC_ScanData_t adcData;

  for (;;) {
    // Wait for ADC data from queue
    if (osMessageQueueGet(adcQueueHandle, &adcData, NULL, osWaitForever) == osOK) {
      
      // Reusable CAN frame for all messages
      CanFrame frame = {0};
      frame.can_bus = 0;
      frame.timestamp = adcData.timestamp;

      // Create CAN frame for coolant and oil temperature sensors (CAN ID 910)
      TEMPERATURES_t tempMsg = {0};
      tempMsg.OilTemp0 = adcData.gearbox_temp[0];
      tempMsg.OilTemp1 = adcData.gearbox_temp[1];
      tempMsg.CoolantTemp0 = adcData.coolant_temp[0];
      tempMsg.CoolantTemp1 = adcData.coolant_temp[1];
      
      if (Pack_TEMPERATURES(&tempMsg, frame.can_data, 8) == STATUS_OK) {
        frame.can_id = 910;
        pushToWSandCAN(&frame, 8);
      }

      // Create CAN frame for potentiometers (CAN ID 911)
      POTENTIOMETERS_t potMsg = {0};
      potMsg.Potentiometer0 = adcData.pot_voltages[0];
      potMsg.Potentiometer1 = adcData.pot_voltages[1];
      
      if (Pack_POTENTIOMETERS(&potMsg, frame.can_data, 8) == STATUS_OK) {
        frame.can_id = 911;
        pushToWSandCAN(&frame, 4);
      }

      // Create CAN frame for strain gauge linkages (CAN ID 913)
      STRAIN_GAUGE_LINKAGES_t strainLinkMsg = {0};
      strainLinkMsg.StrainLinkage0 = (int16_t)adcData.strain_voltages[0];
      strainLinkMsg.StrainLinkage1 = (int16_t)adcData.strain_voltages[1];
      strainLinkMsg.StrainLinkage2 = (int16_t)adcData.strain_voltages[2];
      strainLinkMsg.StrainLinkage3 = (int16_t)adcData.strain_voltages[3];
      
      if (Pack_STRAIN_GAUGE_LINKAGES(&strainLinkMsg, frame.can_data, 8) == STATUS_OK) {
        frame.can_id = 913;
        pushToWSandCAN(&frame, 8);
      }

      // Create CAN frame for strain gauge steering column (CAN ID 915)
      STRAIN_GAUGE_STEERING_t strainSteerMsg = {0};
      strainSteerMsg.StrainSteering0 = (int16_t)adcData.strain_voltages[4];
      strainSteerMsg.StrainSteering1 = (int16_t)adcData.strain_voltages[5];
      
      if (Pack_STRAIN_GAUGE_STEERING(&strainSteerMsg, frame.can_data, 8) == STATUS_OK) {
        frame.can_id = 915;
        pushToWSandCAN(&frame, 4);
      }
    }
  }
}

static void pushToWSandCAN(CanFrame *frame, uint8_t length)
{
  if (osMessageQueueGetSpace(wsCanQueueHandle) > 0) {
    osMessageQueuePut(wsCanQueueHandle, frame, 0, 0);
  }
  else {
    // TODO: Implement error handling for dropped messages
    dropped_packets++;
  }
  if (sendCanFrame(frame->can_id, frame->can_bus, frame->can_data, length) != 0) {
    log_msg(LL_ERR, "Failed to send CAN ID %d through bus %d", frame->can_id, frame->can_bus);
  }
}

