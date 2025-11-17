#ifndef INC_ADC_SCANNER_H_
#define INC_ADC_SCANNER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Defines -------------------------------------------------------------------*/
#define ADC_TIM_CLK 550000000  // Timer clock frequency in Hz
#define ADC_SAMPLE_RATE 10  // ADC sampling rate (Hz)

/* Exported types ------------------------------------------------------------*/
typedef struct {
    // Processed sensor values
    float strain_voltages[6];      // Strain gauge voltages (V)
    float gearbox_temp[2];         // Gearbox oil temperatures (°C)
    float coolant_temp[2];         // Coolant temperatures (°C)
    float pot_voltages[2];          // Potentiometer voltages (V)
    
    uint64_t timestamp;            // Conversion timestamp
} ADC_ScanData_t;

/* Exported functions prototypes ---------------------------------------------*/
void ADC_Scanner_Init(TIM_HandleTypeDef *htim);
void ADC_Scanner_TriggerConversions(void);

extern TIM_HandleTypeDef *ADCTimerInstance;

#ifdef __cplusplus
}
#endif
#endif
