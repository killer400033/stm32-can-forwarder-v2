#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <stdint.h>
#include <stdbool.h>
#include "w5500.hpp"

// Error codes
#define SOCK_OK                  0   // Success
#define SOCKERR_INVALID_PARAM   -1   // Invalid parameter (socket number, null pointer, etc.)
#define SOCKERR_TIMEOUT         -2   // Operation timed out
#define SOCKERR_QUEUE_FULL      -3   // Command Queue is full
#define SOCKERR_INVALID_STATE   -4   // Socket in invalid state
#define SOCKERR_TCP_TIMEOUT     -5   // TCP timeout
#define SOCKERR_BUFFER_FULL     -6   // Buffer is full
#define SOCKERR_BUFFER_TOO_SMALL -7   // Buffer is too small

// External socket array declaration
extern socket_t sockets[_WIZCHIP_SOCK_NUM_];

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize a socket with protocol and port
 * @warning **MUST NOT be called from interrupt context!** Uses blocking operations and taskENTER_CRITICAL().
 * @param sn Socket number (0-7)
 * @param protocol TCP or UDP protocol
 * @param port Source port number
 * @param tx_buf Pointer to TX buffer for this socket
 * @param tx_buf_len Size of TX buffer
 * @param rx_buf Pointer to RX buffer for this socket
 * @param rx_buf_len Size of RX buffer
 * @param callback Callback function for socket events
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int socket(uint8_t sn, socket_protocol_t protocol, uint16_t port, uint8_t* tx_buf, uint16_t tx_buf_len, uint8_t* rx_buf, uint16_t rx_buf_len, socket_callback_t callback=NULL);

/**
 * @brief Connect to a destination (TCP only - sets destination IP and port, then initiates connection)
 * @warning **MUST NOT be called from interrupt context!** Uses blocking operations and taskENTER_CRITICAL().
 * @param sn Socket number (0-7)
 * @param addr Destination IP address (4 bytes)
 * @param port Destination port number
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int connect(uint8_t sn, uint8_t* addr, uint16_t port);

/**
 * @brief Close a socket
 * @warning **MUST NOT be called from interrupt context!** Uses blocking operations and taskENTER_CRITICAL().
 * @param sn Socket number (0-7)
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int close(uint8_t sn);

/**
 * @brief Disconnect a TCP socket
 * @warning **MUST NOT be called from interrupt context!** Uses blocking operations and taskENTER_CRITICAL().
 * @param sn Socket number (0-7)
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int disconnect(uint8_t sn);

/**
 * @brief Send data on a TCP socket
 * @warning **MUST NOT be called from interrupt context!** Uses taskENTER_CRITICAL() and may block.
 * @param sn Socket number (0-7)
 * @param buf Data buffer to send
 * @param len Length of data to send
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int send(uint8_t sn, uint8_t* buf, uint16_t len);

/**
 * @brief Send data on a UDP socket to a specific destination
 * @warning **MUST NOT be called from interrupt context!** Uses taskENTER_CRITICAL() and may block.
 * @param sn Socket number (0-7)
 * @param buf Data buffer to send
 * @param len Length of data to send
 * @param addr Destination IP address (4 bytes)
 * @param port Destination port number
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int sendto(uint8_t sn, uint8_t* buf, uint16_t len, uint8_t* addr, uint16_t port);

#ifdef __cplusplus
}
#endif

#endif // SOCKET_HPP