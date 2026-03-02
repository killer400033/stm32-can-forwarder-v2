#ifndef INC_ADC_SCANNER_H_
#define INC_ADC_SCANNER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported functions prototypes ---------------------------------------------*/
void ADC_Scanner_Init(TIM_HandleTypeDef *htim);
void ADC_Scanner_TriggerConversions(void);

extern TIM_HandleTypeDef *ADCTimerInstance;

#ifdef __cplusplus
}
#endif
#endif
