#include <cstring>
#include "socket.hpp"
#include "w5500.hpp"
#include "cmsis_os.h"
#include "main.h"

socket_t sockets[_WIZCHIP_SOCK_NUM_] = {0};

/**
 * @brief Initialize a socket with protocol and port
 * @warning **MUST NOT be called from interrupt context!** This function uses blocking operations and taskENTER_CRITICAL().
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
int socket(uint8_t sn, socket_protocol_t protocol, uint16_t port, uint8_t* tx_buf, uint16_t tx_buf_len, uint8_t* rx_buf, uint16_t rx_buf_len, socket_callback_t callback=NULL) {
    if (sn >= _WIZCHIP_SOCK_NUM_) {
        return SOCKERR_INVALID_PARAM;
    }

    // Clear socket send and receive queues
    queueClear(&sockets[sn].tx_buf_queue);
    sockets[sn].rx_buf_start_idx = 0;
    sockets[sn].rx_buf_end_idx = 0;

    // Store callback
    sockets[sn].callback = callback;
    
    // Store buffer configuration
    sockets[sn].tx_buf = tx_buf;
    sockets[sn].tx_buf_len = tx_buf_len;
    sockets[sn].rx_buf = rx_buf;
    sockets[sn].rx_buf_len = rx_buf_len;
    
    // Write Sn_MR (Mode Register) - TCP or UDP
    sockets[sn].registers.MR = (uint8_t)protocol;
    if (protocol == SOCKET_PROTOCOL_TCP) {
        sockets[sn].registers.MR |= Sn_MR_ND; // No Delayed ACK(TCP)
    }
    if (!enqueueSetReg(sn, Sn_MR(sn), &sockets[sn].registers.MR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }
    
    // Write Sn_PORT (Source Port) - 2 bytes, big-endian
    sockets[sn].registers.PORT[0] = (port >> 8) & 0xFF;
    sockets[sn].registers.PORT[1] = port & 0xFF;
    if (!enqueueSetReg(sn, Sn_PORT(sn), sockets[sn].registers.PORT, 2)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Write Sn_IMR (Interrupt Mask Register) - 0xFF
    sockets[sn].registers.IMR = 0xFF;
    if (!enqueueSetReg(sn, Sn_IMR(sn), &sockets[sn].registers.IMR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Write Sn_CR (Command Register) - OPEN
    if (!enqueueSetReg(sn, Sn_CR(sn), Sn_CR_OPEN, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    if (protocol == SOCKET_PROTOCOL_TCP) {
        // Poll for socket to be in init state (TCP)
        ret = pollRegNoIT(sn, Sn_SR(sn), &sockets[sn].registers.SR, SOCK_INIT);
        if (ret != SOCK_OK) {
            return ret;
        }
    }
    else if (protocol == SOCKET_PROTOCOL_UDP) {
        // Poll for socket to be in udp state (UDP)
        ret = pollRegNoIT(sn, Sn_SR(sn), &sockets[sn].registers.SR, SOCK_UDP);
        if (ret != SOCK_OK) {
            return ret;
        }
    }
    
    return SOCK_OK;
}

/**
 * @brief Close a socket
 * @warning **MUST NOT be called from interrupt context!** This function uses blocking operations and taskENTER_CRITICAL().
 * @param sn Socket number (0-7)
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int close(uint8_t sn) {
    if (sn >= _WIZCHIP_SOCK_NUM_) {
        return SOCKERR_INVALID_PARAM;
    }
    
    // Write Sn_CR (Command Register) - CLOSE
    if (!enqueueSetReg(sn, Sn_CR(sn), Sn_CR_CLOSE, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Poll for socket to be in closed state
    ret = pollRegNoIT(sn, Sn_SR(sn), &sockets[sn].registers.SR, SOCK_CLOSED);
    if (ret != SOCK_OK) {
        return ret;
    }
    return SOCK_OK;
}

/**
 * @brief Connect to a destination (TCP only - sets destination IP and port, then initiates connection)
 * @warning **MUST NOT be called from interrupt context!** This function uses blocking operations and taskENTER_CRITICAL().
 * @param sn Socket number (0-7)
 * @param addr Destination IP address (4 bytes)
 * @param port Destination port number
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int connect(uint8_t sn, uint8_t* addr, uint16_t port) {
    if (sn >= _WIZCHIP_SOCK_NUM_ || addr == NULL) {
        return SOCKERR_INVALID_PARAM;
    }

    if (sockets[sn].registers.SR != SOCK_INIT) {
        return SOCKERR_INVALID_STATE;
    }

    // Write Sn_DIPR (Destination IP Address) - 4 bytes
    memcpy(sockets[sn].registers.DIPR, addr, 4);
    if (!enqueueSetReg(sn, Sn_DIPR(sn), addr, 4)) {
        return SOCKERR_QUEUE_FULL;
    }
    
    // Write Sn_DPORT (Destination Port) - 2 bytes, big-endian
    uint8_t dport_bytes[2];
    dport_bytes[0] = (port >> 8) & 0xFF;
    dport_bytes[1] = port & 0xFF;
    if (!enqueueSetReg(sn, Sn_DPORT(sn), dport_bytes, 2)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Write Sn_CR (Command Register) - CONNECT
    if (!enqueueSetReg(sn, Sn_CR(sn), Sn_CR_CONNECT, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Poll for socket to exit SOCK_INIT state
    ret = pollRegWithIT(sn, Sn_SR(sn), &sockets[sn].registers.SR, SOCK_INIT, true);
    if (ret != SOCK_OK) {
        return ret;
    }

    // Check if socket is in SOCK_ESTABLISHED state
    if (sockets[sn].registers.SR != SOCK_ESTABLISHED) {
        return SOCKERR_TCP_TIMEOUT;
    }

    return SOCK_OK;
}

/**
 * @brief Disconnect a TCP socket
 * @warning **MUST NOT be called from interrupt context!** This function uses blocking operations and taskENTER_CRITICAL().
 * @param sn Socket number (0-7)
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int disconnect(uint8_t sn) {
    if (sn >= _WIZCHIP_SOCK_NUM_) {
        return SOCKERR_INVALID_PARAM;
    }

    if (sockets[sn].registers.SR != SOCK_ESTABLISHED && sockets[sn].registers.SR != SOCK_CLOSE_WAIT) {
        return SOCKERR_INVALID_STATE;
    }
    
    // Write Sn_CR (Command Register) - DISCONNECT
    if (!enqueueSetReg(sn, Sn_CR(sn), Sn_CR_DISCON, 1)) {
        return SOCKERR_QUEUE_FULL;
    }
    
    // Poll for socket to be in closed state
    ret = pollRegNoIT(sn, Sn_SR(sn), &sockets[sn].registers.SR, SOCK_CLOSED);
    if (ret != SOCK_OK) {
        return ret;
    }

    return SOCK_OK;
}

/**
 * @brief Send data on a TCP socket
 * @warning **MUST NOT be called from interrupt context!** This function uses taskENTER_CRITICAL() and may block.
 * @param sn Socket number (0-7)
 * @param buf Data buffer to send
 * @param len Length of data to send
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int send(uint8_t sn, uint8_t* buf, uint16_t len) {
    if (sn >= _WIZCHIP_SOCK_NUM_ || buf == NULL || len == 0) {
        return SOCKERR_INVALID_PARAM;
    }

    if (sockets[sn].registers.SR != SOCK_ESTABLISHED && sockets[sn].registers.SR != SOCK_CLOSE_WAIT) {
        return SOCKERR_INVALID_STATE;
    }

    if (sockets[sn].registers.TX_BUF_SIZE < len) {
        return SOCKERR_BUFFER_TOO_SMALL;
    }

    // Critical section
    // Ensure queue is not modified or DMA write isn't attempted until we finish
    taskENTER_CRITICAL();

    int16_t write_index = getTXBufferIndex(&sockets[sn], len + 3);
    bool success;
    
    if (write_index != -1) {        
        // Add segment to queue
        buffer_segment_t new_segment;
        new_segment.start_index = write_index;
        new_segment.end_index = write_index + len + 3;
        success = queuePushBack(&sockets[sn].tx_buf_queue, new_segment);

        if (success) {
            memcpy(sockets[sn].tx_buf + write_index + 3, buf, len);
        }
    }

    taskEXIT_CRITICAL();

    if (write_index == -1) {
        return SOCKERR_BUFFER_FULL;
    }

    if (!success) {
        return SOCKERR_QUEUE_FULL;
    }
    return SOCK_OK;
}

/**
 * @brief Send data on a UDP socket to a specific destination
 * @warning **MUST NOT be called from interrupt context!** This function uses taskENTER_CRITICAL() and may block.
 * @param sn Socket number (0-7)
 * @param buf Data buffer to send
 * @param len Length of data to send
 * @param addr Destination IP address (4 bytes)
 * @param port Destination port number
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int sendto(uint8_t sn, uint8_t * buf, uint16_t len, uint8_t *addr, uint16_t port) {
    if (sn >= _WIZCHIP_SOCK_NUM_ || buf == NULL || len == 0 || addr == NULL || port == 0) {
        return SOCKERR_INVALID_PARAM;
    }

    if (sockets[sn].registers.SR != SOCK_UDP) {
        return SOCKERR_INVALID_STATE;
    }

    if (sockets[sn].registers.TX_BUF_SIZE < len) {
        return SOCKERR_BUFFER_TOO_SMALL;
    }
    
    // Critical section
    // Ensure queue is not modified or DMA write isn't attempted until we finish
    taskENTER_CRITICAL();

    int16_t write_index = getTXBufferIndex(&sockets[sn], len + 3);
    bool success;
    
    if (write_index != -1) {
        // Add segment to queue
        buffer_segment_t new_segment;
        new_segment.start_index = write_index;
        new_segment.end_index = write_index + len + 3;
        memcpy(new_segment.addr, addr, 4);
        new_segment.port = port;
        success = queuePushBack(&sockets[sn].tx_buf_queue, new_segment);

        if (success) {
            memcpy(sockets[sn].tx_buf + write_index + 3, buf, len);
        }
    }

    taskEXIT_CRITICAL();

    if (write_index == -1) {
        return SOCKERR_BUFFER_FULL;
    }
    
    if (!success) {
        return SOCKERR_QUEUE_FULL;
    }
    return SOCK_OK;
}