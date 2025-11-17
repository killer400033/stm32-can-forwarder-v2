#include "unix_time.h"
#include "ntp_client.h"
#include "w5500_driver.h"
#include "dns_resolve.h"
#include "cmsis_os.h"
#include "log_handler.h"
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

extern RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef *UNIXTimerInstance;
volatile uint32_t seconds;

// NTP client variables
static bool ntp_initialized = false;
bool ntp_sync_successful = false;
static bool ntp_dns_request_pending = false;

// DNS queue handle
extern osMessageQueueId_t dnsReqQueueHandle;

osThreadId_t ntpSyncTaskHandle;
const osThreadAttr_t ntpSyncTask_attributes = {
  .name = "NTP Sync",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void ntpSyncThread(void *argument);
static void setUnixTimeSeconds(uint32_t time);
static time_t getUnixFromRTC(RTC_TimeTypeDef sTime, RTC_DateTypeDef sDate);
static void getRTCFromUnix(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate, time_t unixTime);
static int8_t initNTPClient(const uint8_t* ntp_server_ip);
static uint32_t getNTPTime(void);
static void ntpDnsCallback(const uint8_t* ip_addr, uint32_t ttl, bool success);
static void updateInternalTime(uint32_t ntp_time);

void initTime(TIM_HandleTypeDef *htim) {
	UNIXTimerInstance = htim;
	// Period (ARR) = 1,000,000 - 1 (so it overflows after 1 million ticks = 1 second)
	htim->Instance->PSC = (UNIX_TIM_CLK / 1000000) - 1;
	htim->Instance->ARR = 1000000 - 1;
	htim->Instance->CNT = 0;

	// Generate update event to load the prescaler value
	htim->Instance->EGR = TIM_EGR_UG;

	// Start timer with update (overflow) interrupt
	HAL_TIM_Base_Start_IT(htim);

	// Set Unix time to RTC
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	setUnixTimeSeconds(getUnixFromRTC(sTime, sDate));

	ntpSyncTaskHandle = osThreadNew(ntpSyncThread, NULL, &ntpSyncTask_attributes);
}

void ntpSyncThread(void *argument) {
	for (;;) {

		if (!ntp_dns_request_pending && !ntp_sync_successful) {
			if (osMessageQueueGetSpace(dnsReqQueueHandle) > 0) {
				dns_request_t dns_req = {
					.domain_name = NTP_SERVER_DOMAIN,
					.callback = ntpDnsCallback
				};

				if (osMessageQueuePut(dnsReqQueueHandle, &dns_req, 0, 0) == osOK) {
					ntp_dns_request_pending = true;
				} else {
					log_msg(LL_ERR, "NTP DNS Request: Failed to queue request");
				}
			} else {
				log_msg(LL_ERR, "NTP DNS Request: Queue full, cannot queue request");
			}
		}

		if (ntp_sync_successful) {
			log_msg(LL_DBG, "NTP sync successful, terminating NTP sync thread");
			osThreadExit();
		}

		osDelay(10000);
	}
}

// DNS callback for NTP server resolution
static void ntpDnsCallback(const uint8_t* ip_addr, uint32_t ttl, bool success) {
	ntp_dns_request_pending = false;
	ntp_sync_successful = false;
	
	if (!success || ip_addr == NULL) {
		updateInternalTime(0);
		return;
	}
	
	// Initialize NTP client with resolved IP
	if (initNTPClient(ip_addr) == NTP_OK) {
		log_msg(LL_DBG, "NTP Init Successful!");
		uint32_t ntp_time = getNTPTime();
		if (ntp_time != 0) {
			updateInternalTime(ntp_time);
			ntp_sync_successful = true;
		} else {
			updateInternalTime(0);
		}
	} else {
		updateInternalTime(0);
	}
}

static void setUnixTimeSeconds(uint32_t time) {
	seconds = time;
}

// Timer overflow callback
void UNIX_Timer_Callback(void) {
	seconds++; // This has a very unlikely bug where interrupt is called while getting time, causing incorrect time.
}

// Returns the current time in UNIX form in seconds
uint32_t getUnixTimeSeconds() {
	return seconds;
}

// Returns the current time in UNIX form in microseconds
uint64_t getUnixTimeMicroseconds() {
	return (uint64_t)seconds * 1000000U + (uint64_t)(UNIXTimerInstance->Instance->CNT);
}

// Returns the current time in UNIX form in nanoseconds
uint64_t getUnixTimeNanoseconds() {
	return ((uint64_t)seconds * 1000000U + (uint64_t)(UNIXTimerInstance->Instance->CNT)) * 1000U;
}

static time_t getUnixFromRTC(RTC_TimeTypeDef sTime, RTC_DateTypeDef sDate) {
	struct tm calenderTime;
	calenderTime.tm_year = sDate.Year + 100;
	calenderTime.tm_mon = sDate.Month - 1;
	calenderTime.tm_mday = sDate.Date;
	calenderTime.tm_hour = sTime.Hours;
	calenderTime.tm_min = sTime.Minutes;
	calenderTime.tm_sec = sTime.Seconds;
	calenderTime.tm_isdst = -1;

	time_t unixTime = mktime(&calenderTime);
	return unixTime - UTC_UNIX_OFFSET;
}

static void getRTCFromUnix(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate, time_t unixTime) {
	unixTime += UTC_UNIX_OFFSET;
	struct tm *calenderTime = gmtime(&unixTime);

	sDate->Year = calenderTime->tm_year - 100;
	sDate->Month = calenderTime->tm_mon + 1;
	sDate->Date = calenderTime->tm_mday;
	sTime->Hours = calenderTime->tm_hour;
	sTime->Minutes = calenderTime->tm_min;
	sTime->Seconds = calenderTime->tm_sec;
}

// Initialize NTP client with resolved IP address
static int8_t initNTPClient(const uint8_t* ntp_server_ip) {
	ntp_config_t config = {
		.ntp_server = {0},
		.timeout_ms = 5000,
		.max_retries = NTP_RETRY_CNT,
		.socket_num = NTP_SOCKET,
		.version = 4
	};

	if (ntp_server_ip != NULL) {
		memcpy(config.ntp_server, ntp_server_ip, 4);
	} else {
		return NTP_ERROR_INVALID_PARAM;
	}

	int8_t result = ntp_init(&config);
	if (result == NTP_OK) {
		ntp_initialized = true;
	}

	return result;
}

// Synchronize time with NTP server
static uint32_t getNTPTime(void) {
	if (!ntp_initialized) {
		return 0;
	}

	uint32_t ntp_timestamp;
	int8_t result = ntp_get_time(&ntp_timestamp);

	if (result == NTP_OK) {
		return ntp_timestamp;
	}

	return 0;
}

static void updateInternalTime(uint32_t ntp_time) {
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	
	if (ntp_time == 0) {
		log_msg(LL_WRN, "Failed to get NTP Time...");
	}
	else {
		getRTCFromUnix(&sTime, &sDate, ntp_time);
		HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		setUnixTimeSeconds(getUnixFromRTC(sTime, sDate));

		log_msg(LL_DBG, "Successfully got NTP time!");
		log_msg(LL_DBG, "Current Unix Time: %ld", getUnixTimeSeconds());
	}
}


void getCurrentDateTime(RTC_TimeTypeDef* sTime, RTC_DateTypeDef* sDate)
{
	if (sTime != NULL && sDate != NULL) {
		HAL_RTC_GetTime(&hrtc, sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, sDate, RTC_FORMAT_BIN);
	}
}
