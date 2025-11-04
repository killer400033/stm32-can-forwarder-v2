#include "adc_scanner.h"
#include "main.h"
#include <string.h>
#include <stdbool.h>
#include "unix_time.h"
#include "cmsis_os.h"
#include "forwarder_pb.pb.h"
#include "can_driver.h"

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
        // TODO: Implement error handling for dropped messages
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
      
      // Create CAN frame for coolant and oil temperature sensors (CAN ID 910)
      CanFrame tempFrame = {0};
      tempFrame.can_id = 910;
      tempFrame.can_bus = 0;  // Using CAN bus 0 (hfdcan1)
      tempFrame.timestamp = adcData.timestamp;
      
      // Scale temperatures to 0.01°C resolution and pack as 16-bit values
      int16_t oil_temp_0 = (int16_t)(adcData.gearbox_temp[0] / 0.01f);
      int16_t oil_temp_1 = (int16_t)(adcData.gearbox_temp[1] / 0.01f);
      int16_t coolant_temp_0 = (int16_t)(adcData.coolant_temp[0] / 0.01f);
      int16_t coolant_temp_1 = (int16_t)(adcData.coolant_temp[1] / 0.01f);
      
      tempFrame.can_data[0] = (oil_temp_0 >> 8) & 0xFF;
      tempFrame.can_data[1] = oil_temp_0 & 0xFF;
      tempFrame.can_data[2] = (oil_temp_1 >> 8) & 0xFF;
      tempFrame.can_data[3] = oil_temp_1 & 0xFF;
      tempFrame.can_data[4] = (coolant_temp_0 >> 8) & 0xFF;
      tempFrame.can_data[5] = coolant_temp_0 & 0xFF;
      tempFrame.can_data[6] = (coolant_temp_1 >> 8) & 0xFF;
      tempFrame.can_data[7] = coolant_temp_1 & 0xFF;
      
      // Push temperature frame to WebSocket CAN queue
      if (osMessageQueueGetSpace(wsCanQueueHandle) > 0) {
        osMessageQueuePut(wsCanQueueHandle, &tempFrame, 0, 0);
      }
      
      // Send temperature frame through CAN bus
      if (sendCanFrame(tempFrame.can_id, tempFrame.can_bus, tempFrame.can_data, 8) != 0) {
        // TODO: Implement error handling for failed CAN transmission
      }

      // Create CAN frame for potentiometers (CAN ID 911)
      CanFrame potFrame = {0};
      potFrame.can_id = 911;
      potFrame.can_bus = 0;  // Using CAN bus 0 (hfdcan1)
      potFrame.timestamp = adcData.timestamp;
      
      // Scale potentiometer voltages to 0.001V resolution and pack as 16-bit values
      int16_t pot_0 = (int16_t)(adcData.pot_voltages[0] / 0.001f);
      int16_t pot_1 = (int16_t)(adcData.pot_voltages[1] / 0.001f);
      
      potFrame.can_data[0] = (pot_0 >> 8) & 0xFF;
      potFrame.can_data[1] = pot_0 & 0xFF;
      potFrame.can_data[2] = (pot_1 >> 8) & 0xFF;
      potFrame.can_data[3] = pot_1 & 0xFF;
      
      // Push potentiometer frame to WebSocket CAN queue
      if (osMessageQueueGetSpace(wsCanQueueHandle) > 0) {
        osMessageQueuePut(wsCanQueueHandle, &potFrame, 0, 0);
      }
      
      // Send potentiometer frame through CAN bus
      if (sendCanFrame(potFrame.can_id, potFrame.can_bus, potFrame.can_data, 4) != 0) {
        // TODO: Implement error handling for failed CAN transmission
      }

      // Create CAN frame for strain gauge linkages (CAN ID 913)
      CanFrame strainLinkFrame = {0};
      strainLinkFrame.can_id = 913;
      strainLinkFrame.can_bus = 0;  // Using CAN bus 0 (hfdcan1)
      strainLinkFrame.timestamp = adcData.timestamp;
      
      // Scale strain voltages to 1V resolution and pack as 16-bit values
      int16_t strain_0 = (int16_t)(adcData.strain_voltages[0] / 1.0f);
      int16_t strain_1 = (int16_t)(adcData.strain_voltages[1] / 1.0f);
      int16_t strain_2 = (int16_t)(adcData.strain_voltages[2] / 1.0f);
      int16_t strain_3 = (int16_t)(adcData.strain_voltages[3] / 1.0f);
      
      strainLinkFrame.can_data[0] = (strain_0 >> 8) & 0xFF;
      strainLinkFrame.can_data[1] = strain_0 & 0xFF;
      strainLinkFrame.can_data[2] = (strain_1 >> 8) & 0xFF;
      strainLinkFrame.can_data[3] = strain_1 & 0xFF;
      strainLinkFrame.can_data[4] = (strain_2 >> 8) & 0xFF;
      strainLinkFrame.can_data[5] = strain_2 & 0xFF;
      strainLinkFrame.can_data[6] = (strain_3 >> 8) & 0xFF;
      strainLinkFrame.can_data[7] = strain_3 & 0xFF;
      
      // Push strain linkage frame to WebSocket CAN queue
      if (osMessageQueueGetSpace(wsCanQueueHandle) > 0) {
        osMessageQueuePut(wsCanQueueHandle, &strainLinkFrame, 0, 0);
      }
      
      // Send strain linkage frame through CAN bus
      if (sendCanFrame(strainLinkFrame.can_id, strainLinkFrame.can_bus, strainLinkFrame.can_data, 8) != 0) {
        // TODO: Implement error handling for failed CAN transmission
      }

      // Create CAN frame for strain gauge steering column (CAN ID 915)
      CanFrame strainSteerFrame = {0};
      strainSteerFrame.can_id = 915;
      strainSteerFrame.can_bus = 0;  // Using CAN bus 0 (hfdcan1)
      strainSteerFrame.timestamp = adcData.timestamp;
      
      // Scale strain voltages to 1V resolution and pack as 16-bit values
      int16_t strain_4 = (int16_t)(adcData.strain_voltages[4] / 1.0f);
      int16_t strain_5 = (int16_t)(adcData.strain_voltages[5] / 1.0f);
      
      strainSteerFrame.can_data[0] = (strain_4 >> 8) & 0xFF;
      strainSteerFrame.can_data[1] = strain_4 & 0xFF;
      strainSteerFrame.can_data[2] = (strain_5 >> 8) & 0xFF;
      strainSteerFrame.can_data[3] = strain_5 & 0xFF;
      
      // Push strain steering frame to WebSocket CAN queue
      if (osMessageQueueGetSpace(wsCanQueueHandle) > 0) {
        osMessageQueuePut(wsCanQueueHandle, &strainSteerFrame, 0, 0);
      }
      
      // Send strain steering frame through CAN bus
      if (sendCanFrame(strainSteerFrame.can_id, strainSteerFrame.can_bus, strainSteerFrame.can_data, 4) != 0) {
        // TODO: Implement error handling for failed CAN transmission
      }
    }
  }
}

