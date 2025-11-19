#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>
#include <stdbool.h>
#include "queue.hpp"

#define COMMAND_QUEUE_SIZE 100
#define SOCKET_QUEUE_SIZE 15

#define MAC_ADDRESS {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define INTLEVEL 0x0000
#define RETRY_TIME 1000 // Retry time in milliseconds
#define RETRY_COUNT 5 // Retry count
#define KEEP_ALIVE_TIMER 15 // Keep-alive timer in seconds (set to 0 to disable)

// Error codes
#define SOCK_OK                  0   // Success
#define SOCKERR_INVALID_PARAM   -1   // Invalid parameter (socket number, null pointer, etc.)
#define SOCKERR_TIMEOUT         -2   // Operation timed out
#define SOCKERR_QUEUE_FULL      -3   // Command Queue is full
#define SOCKERR_INVALID_STATE   -4   // Socket in invalid state
#define SOCKERR_TCP_TIMEOUT     -5   // TCP timeout
#define SOCKERR_BUFFER_FULL     -6   // Buffer is full

typedef enum {
    SOCKET_PROTOCOL_TCP = Sn_MR_TCP,
    SOCKET_PROTOCOL_UDP = Sn_MR_UDP
} socket_protocol_t;

typedef struct {
    bool is_addr;
    union {
        uint8_t inline_buf[9];  // used when is_addr == false
        const uint8_t *ptr;      // used when is_addr == true
    } data;
    uint16_t len;
} command_t;

typedef struct
{
    uint8_t MR;          // 0x0000 Mode

    uint8_t GAR[4];      // 0x0001–0x0004 Gateway IP
    uint8_t SUBR[4];     // 0x0005–0x0008 Subnet mask
    uint8_t SHAR[6];     // 0x0009–0x000E Source MAC
    uint8_t SIPR[4];     // 0x000F–0x0012 Source IP

    uint8_t INTLEVEL[2]; // 0x0013–0x0014 Interrupt Low Level Timer (INTLEVEL0/1)
    uint8_t IR;          // 0x0015 Interrupt
    uint8_t IMR;         // 0x0016 Interrupt Mask
    uint8_t SIR;         // 0x0017 Socket Interrupt
    uint8_t SIMR;        // 0x0018 Socket Interrupt Mask

    uint8_t RTR[2];      // 0x0019–0x001A Retry Time
    uint8_t RCR;         // 0x001B Retry Count
} common_regs_t;

#pragma pack(push, 1)
typedef struct {
    uint8_t  MR;              // 0x0000  Sn_MR
    uint8_t  CR;              // 0x0001  Sn_CR
    uint8_t  IR;              // 0x0002  Sn_IR
    uint8_t  SR;              // 0x0003  Sn_SR

    uint8_t  PORT[2];         // 0x0004–0x0005  Sn_PORT0/1 (big-endian)
    uint8_t  DHAR[6];         // 0x0006–0x000B  Sn_DHAR0..5
    uint8_t  DIPR[4];         // 0x000C–0x000F  Sn_DIPR0..3
    uint8_t  DPORT[2];        // 0x0010–0x0011  Sn_DPORT0/1
    uint8_t  MSSR[2];         // 0x0012–0x0013  Sn_MSSR0/1

    uint8_t  _res_0014;       // 0x0014  Reserved

    uint8_t  TOS;             // 0x0015  Sn_TOS
    uint8_t  TTL;             // 0x0016  Sn_TTL

    uint8_t  _res_0017_001D[7]; // 0x0017–0x001D  Reserved

    uint8_t  RXBUF_SIZE;      // 0x001E  Sn_RXBUF_SIZE
    uint8_t  TXBUF_SIZE;      // 0x001F  Sn_TXBUF_SIZE

    uint8_t  TX_FSR[2];       // 0x0020–0x0021  Sn_TX_FSR0/1
    uint8_t  TX_RD[2];        // 0x0022–0x0023  Sn_TX_RD0/1
    uint8_t  TX_WR[2];        // 0x0024–0x0025  Sn_TX_WR0/1
    uint8_t  RX_RSR[2];       // 0x0026–0x0027  Sn_RX_RSR0/1
    uint8_t  RX_RD[2];        // 0x0028–0x0029  Sn_RX_RD0/1
    uint8_t  RX_WR[2];        // 0x002A–0x002B  Sn_RX_WR0/1

    uint8_t  IMR;             // 0x002C  Sn_IMR
    uint8_t  FRAG[2];         // 0x002D–0x002E  Sn_FRAG0/1
    uint8_t  KPALVTR;         // 0x002F  Sn_KPALVTR
} socket_regs_t;
#pragma pack(pop)

struct BufferSegment {
    uint16_t start_index;  // Start index of data segment in buffer
    uint16_t end_index;    // End index of data segment in buffer (exclusive)

    // Used for UDP packets
    uint8_t addr[4];         // Destination IP address
    uint16_t port;         // Destination port number
};

typedef struct {
    socket_regs_t registers;
    uint8_t* data_buffer;
    uint16_t data_buffer_size;
    Queue<BufferSegment, SOCKET_QUEUE_SIZE> data_queue;
    bool is_sending;
} socket_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize W5500 chip with network configuration
 * @param ip_address IP address (4 bytes)
 * @param subnet_mask Subnet mask (4 bytes)
 * @param gateway_ip Gateway IP address (4 bytes)
 * @param rx_buf_sizes RX buffer sizes for all 8 sockets (in KB)
 * @param tx_buf_sizes TX buffer sizes for all 8 sockets (in KB)
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int initWizchip(uint8_t* ip_address, uint8_t* subnet_mask, uint8_t* gateway_ip, 
                const uint8_t* rx_buf_sizes, const uint8_t* tx_buf_sizes);

/**
 * @brief Initialize a socket with protocol and port
 * @param sn Socket number (0-7)
 * @param protocol TCP or UDP protocol
 * @param port Source port number
 * @param data_buffer Pointer to data buffer for this socket (optional)
 * @param data_buffer_size Size of data buffer
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int socket(uint8_t sn, socket_protocol_t protocol, uint16_t port, uint8_t* data_buffer=NULL, uint16_t data_buffer_size=0);

/**
 * @brief Connect to a destination (TCP only - sets destination IP and port, then initiates connection)
 * @param sn Socket number (0-7)
 * @param addr Destination IP address (4 bytes)
 * @param port Destination port number
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int connect(uint8_t sn, uint8_t* addr, uint16_t port);

/**
 * @brief Close a socket
 * @param sn Socket number (0-7)
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int close(uint8_t sn);

/**
 * @brief Disconnect a TCP socket
 * @param sn Socket number (0-7)
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int disconnect(uint8_t sn);

/**
 * @brief Send data on a TCP socket
 * @param sn Socket number (0-7)
 * @param buf Data buffer to send
 * @param len Length of data to send
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int send(uint8_t sn, uint8_t* buf, uint16_t len);

/**
 * @brief Send data on a UDP socket to a specific destination
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

#endif