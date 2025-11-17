#include "dns_resolve.h"
#include "cmsis_os.h"
#include "dns_client.h"
#include "log_handler.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <w5500_driver.h>

void dnsResolveThread(void *argument);

osThreadId_t dnsResolveTaskHandle;
const osThreadAttr_t dnsResolveTask_attributes = {
  .name = "DNS Resolve",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

// Queue handle - will be created externally
extern osMessageQueueId_t dnsReqQueueHandle;

void initDNSResolve() {
    dns_config_t config = {
        .dns_server = {8, 8, 8, 8},
        .timeout_ms = 5000,
        .max_retries = 3,
        .socket_num = DNS_SOCKET
    };

    int8_t result = dns_init(&config);

    if (result == DNS_OK) {
    	log_msg(LL_DBG, "DNS Init Successful!");
    	dnsResolveTaskHandle = osThreadNew(dnsResolveThread, NULL, &dnsResolveTask_attributes);
    }
    else {
    	log_msg(LL_ERR, "DNS Init Failed...");
    }
}

void dnsResolveThread(void *argument) {
	dns_request_t request;
	
	for (;;) {
		// Wait for a request from the queue
		if (osMessageQueueGet(dnsReqQueueHandle, &request, NULL, osWaitForever) == osOK) {
			// Validate request
			if (request.domain_name == NULL || request.callback == NULL) {
				log_msg(LL_ERR, "DNS Request: Invalid request parameters");
				continue;
			}
			
			// Perform DNS resolution
			uint8_t ip_addr[4];
			uint32_t ttl = 0;
			int8_t result = dns_resolve_a(request.domain_name, ip_addr, &ttl);
			
			// Call callback with results
			if (result == DNS_OK) {
				log_msg(LL_DBG, "DNS Request Success: %s -> %d.%d.%d.%d (TTL: %lu)", 
				       request.domain_name, ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3], 
				       (unsigned long)ttl);
				request.callback(ip_addr, ttl, true);
			}
			else {
				log_msg(LL_WRN, "DNS Request Failed: %s", request.domain_name);
				// Call callback with failure (ip_addr can be zero, ttl is 0)
				memset(ip_addr, 0, sizeof(ip_addr));
				request.callback(ip_addr, 0, false);
			}
		}
		
		osThreadYield();
	}
}
