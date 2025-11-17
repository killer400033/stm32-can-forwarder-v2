#ifndef INC_UNIX_TIME_H_
#define INC_UNIX_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "cmsis_os.h"
#include <stdbool.h>
#include "main.h"

#define NTP_RETRY_CNT 5
#define UTC_OFFSET +1000
#define UTC_UNIX_OFFSET (UTC_OFFSET/100)*3600 + (UTC_OFFSET%100)*60 // Calculates UTC offset in seconds
#define UNIX_TIM_CLK 550000000

#define NTP_SERVER_DOMAIN "time.nist.gov"

void initTime(TIM_HandleTypeDef *htim);

extern TIM_HandleTypeDef *UNIXTimerInstance;
extern bool ntp_sync_successful;

uint32_t getUnixTimeSeconds();
uint64_t getUnixTimeMicroseconds();
uint64_t getUnixTimeNanoseconds();
void UNIX_Timer_Callback(void);
void getCurrentDateTime(RTC_TimeTypeDef* sTime, RTC_DateTypeDef* sDate);

#ifdef __cplusplus
}
#endif
#endif
