#ifndef __DNS_RESOLVE__H__
#define __DNS_RESOLVE__H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Callback function type for DNS resolution completion
 * @param ip_addr Pointer to 4-byte IPv4 address array
 * @param ttl Time to live value from DNS response
 * @param success True if resolution succeeded, false otherwise
 */
typedef void (*dns_resolve_callback_t)(const uint8_t* ip_addr, uint32_t ttl, bool success);

/**
 * @brief DNS resolution request structure for queue
 */
typedef struct {
    const char* domain_name;           ///< Domain name to resolve (null-terminated string)
    dns_resolve_callback_t callback;    ///< Callback function to call when resolution completes
} dns_request_t;

void initDNSResolve();

#ifdef __cplusplus
}
#endif
#endif
