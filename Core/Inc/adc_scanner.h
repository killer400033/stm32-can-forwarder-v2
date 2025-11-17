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


/* Exported functions prototypes ---------------------------------------------*/
void ADC_Scanner_Init(TIM_HandleTypeDef *htim);
void ADC_Scanner_TriggerConversions(void);

extern TIM_HandleTypeDef *ADCTimerInstance;

#ifdef __cplusplus
}
#endif
#endif
