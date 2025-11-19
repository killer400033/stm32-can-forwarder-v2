#include <queue>
#include <cstring>
#include "socket.hpp"
#include "queue.hpp"
#include "w5500.h"
#include "cmsis_os.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

static Queue<command_t, COMMAND_QUEUE_SIZE> command_queue;

socket_t sockets[_WIZCHIP_SOCK_NUM_];
common_regs_t common_regs;

static bool enqueueSetRegInline(uint32_t addr, const uint8_t* data, uint8_t len);
static bool enqueueGetRegInline(uint32_t addr, uint8_t* buffer, uint8_t len);
static int pollRegNoIT(uint32_t addr, const uint8_t* reg, uint16_t val, bool inv=false, uint16_t timeout=1000)
static int pollRegWithIT(uint32_t addr, const uint8_t* reg, uint16_t val, bool inv=false);
static uint16_t getDataBufferIndex(socket_t* socket, uint16_t data_length);

int initWizchip(uint8_t* ip_address, uint8_t* subnet_mask, uint8_t* gateway_ip,
                const uint8_t* rx_buf_sizes, const uint8_t* tx_buf_sizes) {
    const uint8_t mac_addr[] = MAC_ADDRESS;
    const uint16_t intlevel = INTLEVEL;
    const uint16_t rtr = RETRY_TIME * 10;
    const uint8_t rcr = RETRY_COUNT;

    queueInit(&command_queue);

    // Write reset command
    common_regs.MR = MR_RST;
    if (!enqueueSetRegInline(MR, &common_regs.MR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Poll for reset to complete
    int ret = pollRegNoIT(MR, &common_regs.MR, 0x00);
    if (ret != SOCK_OK) {
        return ret;
    }
    
    // Write Network Information
    common_regs.GAR = gateway_ip;
    if (!enqueueSetRegInline(GAR, common_regs.GAR, 4)) {
        return SOCKERR_QUEUE_FULL;
    }
    common_regs.SUBR = subnet_mask;
    if (!enqueueSetRegInline(SUBR, common_regs.SUBR, 4)) {
        return SOCKERR_QUEUE_FULL;
    }
    common_regs.SHAR = mac_addr;
    if (!enqueueSetRegInline(SHAR, common_regs.SHAR, 6)) {
        return SOCKERR_QUEUE_FULL;
    }
    common_regs.SIPR = ip_address;
    if (!enqueueSetRegInline(SIPR, common_regs.SIPR, 4)) {
        return SOCKERR_QUEUE_FULL;
    }
    
    // Write INTLEVEL
    common_regs.INTLEVEL[0] = (intlevel >> 8) & 0xFF;
    common_regs.INTLEVEL[1] = intlevel & 0xFF;
    if (!enqueueSetRegInline(INTLEVEL, common_regs.INTLEVEL, 2)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Enable interrupts for all sockets
    common_regs.SIMR = 0xFF;
    if (!enqueueSetRegInline(SIMR, &common_regs.SIMR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }
    
    // Write RTR
    common_regs.RTR[0] = (rtr >> 8) & 0xFF;
    common_regs.RTR[1] = rtr & 0xFF;
    if (!enqueueSetRegInline(W5500_RTR, common_regs.RTR, 2)) {
        return SOCKERR_QUEUE_FULL;
    }
    
    // Write RCR
    common_regs.RCR = rcr;
    if (!enqueueSetRegInline(W5500_RCR, common_regs.RCR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Write buffer sizes for all 8 sockets
    for (uint8_t i = 0; i < _WIZCHIP_SOCK_NUM_; i++) {
        // Write Sn_RXBUF_SIZE (RX Buffer Size) - 1 byte
        sockets[i].registers.RXBUF_SIZE = rx_buf_sizes[i];
        if (!enqueueSetRegInline(Sn_RXBUF_SIZE(i), &sockets[i].registers.RXBUF_SIZE, 1)) {
            return SOCKERR_QUEUE_FULL;
        }
        
        // Write Sn_TXBUF_SIZE (TX Buffer Size) - 1 byte
        sockets[i].registers.TXBUF_SIZE = tx_buf_sizes[i];
        if (!enqueueSetRegInline(Sn_TXBUF_SIZE(i), &sockets[i].registers.TXBUF_SIZE, 1)) {
            return SOCKERR_QUEUE_FULL;
        }
    }

    return SOCK_OK;
}


int socket(uint8_t sn, socket_protocol_t protocol, uint16_t port, uint8_t* data_buffer=NULL, uint16_t data_buffer_size=0) {
    if (sn >= _WIZCHIP_SOCK_NUM_) {
        return SOCKERR_INVALID_PARAM;
    }
    
    // Store buffer configuration
    sockets[sn].data_buffer = data_buffer;
    sockets[sn].data_buffer_size = data_buffer_size;
    
    // Write Sn_MR (Mode Register) - TCP or UDP
    sockets[sn].registers.MR = (uint8_t)protocol;
    if (protocol == SOCKET_PROTOCOL_TCP) {
        sockets[sn].registers.MR |= Sn_MR_ND; // No Delayed ACK(TCP)
    }
    if (!enqueueSetRegInline(Sn_MR(sn), &sockets[sn].registers.MR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }
    
    // Write Sn_PORT (Source Port) - 2 bytes, big-endian
    sockets[sn].registers.PORT[0] = (port >> 8) & 0xFF;
    sockets[sn].registers.PORT[1] = port & 0xFF;
    if (!enqueueSetRegInline(Sn_PORT(sn), sockets[sn].registers.PORT, 2)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Write Sn_IMR (Interrupt Mask Register) - 0xFF
    sockets[sn].registers.IMR = 0xFF;
    if (!enqueueSetRegInline(Sn_IMR(sn), &sockets[sn].registers.IMR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Write Sn_CR (Command Register) - OPEN
    sockets[sn].registers.CR = Sn_CR_OPEN;
    if (!enqueueSetRegInline(Sn_CR(sn), &sockets[sn].registers.CR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Poll for command to be accepted
    int ret = pollRegNoIT(Sn_CR(sn), &sockets[sn].registers.CR, 0x00);
    if (ret != SOCK_OK) {
        return ret;
    }

    if (protocol == SOCKET_PROTOCOL_TCP) {
        // Poll for socket to be in init state (TCP)
        ret = pollRegNoIT(Sn_SR(sn), &sockets[sn].registers.SR, SOCK_INIT);
        if (ret != SOCK_OK) {
            return ret;
        }
    }
    else if (protocol == SOCKET_PROTOCOL_UDP) {
        // Poll for socket to be in udp state (UDP)
        ret = pollRegNoIT(Sn_SR(sn), &sockets[sn].registers.SR, SOCK_UDP);
        if (ret != SOCK_OK) {
            return ret;
        }
    }
    
    return SOCK_OK;
}

int close(uint8_t sn) {
    if (sn >= _WIZCHIP_SOCK_NUM_) {
        return SOCKERR_INVALID_PARAM;
    }
    
    // Write Sn_CR (Command Register) - CLOSE
    sockets[sn].registers.CR = Sn_CR_CLOSE;
    if (!enqueueSetRegInline(Sn_CR(sn), &sockets[sn].registers.CR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Poll for command to be accepted
    int ret = pollRegNoIT(Sn_CR(sn), &sockets[sn].registers.CR, 0x00);
    if (ret != SOCK_OK) {
        return ret;
    }

    // Poll for socket to be in closed state
    ret = pollRegNoIT(Sn_SR(sn), &sockets[sn].registers.SR, SOCK_CLOSED);
    if (ret != SOCK_OK) {
        return ret;
    }
    return SOCK_OK;
}

int connect(uint8_t sn, uint8_t* addr, uint16_t port) {
    if (sn >= _WIZCHIP_SOCK_NUM_ || addr == NULL) {
        return SOCKERR_INVALID_PARAM;
    }

    if (sockets[sn].registers.CR != SOCK_INIT) {
        return SOCKERR_INVALID_STATE;
    }

    // Write Sn_DIPR (Destination IP Address) - 4 bytes
    if (!enqueueSetRegInline(Sn_DIPR(sn), addr, 4)) {
        return SOCKERR_QUEUE_FULL;
    }
    
    // Write Sn_DPORT (Destination Port) - 2 bytes, big-endian
    uint8_t dport_bytes[2];
    dport_bytes[0] = (port >> 8) & 0xFF;
    dport_bytes[1] = port & 0xFF;
    if (!enqueueSetRegInline(Sn_DPORT(sn), dport_bytes, 2)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Write Sn_CR (Command Register) - CONNECT
    sockets[sn].registers.CR = Sn_CR_CONNECT;
    if (!enqueueSetRegInline(Sn_CR(sn), &sockets[sn].registers.CR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Poll for command to be accepted
    int ret = pollRegNoIT(Sn_CR(sn), &sockets[sn].registers.CR, 0x00);
    if (ret != SOCK_OK) {
        return ret;
    }

    // Poll for socket to exit SOCK_INIT state
    ret = pollRegWithIT(Sn_SR(sn), &sockets[sn].registers.SR, SOCK_INIT, true);
    if (ret != SOCK_OK) {
        return ret;
    }

    // Check if socket is in SOCK_ESTABLISHED state
    if (sockets[sn].registers.SR != SOCK_ESTABLISHED) {
        return SOCKERR_TCP_TIMEOUT;
    }

    return SOCK_OK;
}

int disconnect(uint8_t sn) {
    if (sn >= _WIZCHIP_SOCK_NUM_) {
        return SOCKERR_INVALID_PARAM;
    }

    if (sockets[sn].registers.SR != SOCK_ESTABLISHED) {
        return SOCKERR_INVALID_STATE;
    }
    
    // Write Sn_CR (Command Register) - DISCONNECT
    sockets[sn].registers.CR = Sn_CR_DISCONNECT;
    if (!enqueueSetRegInline(Sn_CR(sn), &sockets[sn].registers.CR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Poll for command to be accepted
    int ret = pollRegNoIT(Sn_CR(sn), &sockets[sn].registers.CR, 0x00);
    if (ret != SOCK_OK) {
        return ret;
    }
    
    // Poll for interrupt to be generated
    int ret = pollRegWithIT(Sn_IR(sn), &sockets[sn].registers.IR, Sn_IR_DISCON | Sn_IR_TIMEOUT);
    if (ret != SOCK_OK) {
        return ret;
    }

    if (sockets[sn].registers.IR & Sn_IR_DISCON) {
        return SOCK_OK;
    }
    
    return SOCKERR_TCP_TIMEOUT;
}

// **DO NOT CALL FROM INTERRUPT CONTEXT!**
int send(uint8_t sn, uint8_t* buf, uint16_t len) {
    if (sn >= _WIZCHIP_SOCK_NUM_ || buf == NULL || len == 0) {
        return SOCKERR_INVALID_PARAM;
    }

    if (sockets[sn].registers.SR != SOCK_ESTABLISHED && sockets[sn].registers.SR != SOCK_CLOSE_WAIT) {
        return SOCKERR_INVALID_STATE;
    }

    // Critical section
    // Ensure queue is not modified or DMA write isn't attempted until we finish
    taskENTER_CRITICAL();

    uint16_t write_index = getDataBufferIndex(&sockets[sn], len + 3);
    if (write_index == -1) {
        return SOCKERR_BUFFER_FULL;
    }

    // Add segment to queue
    BufferSegment new_segment;
    new_segment.start_index = write_index;
    new_segment.end_index = write_index + len + 3;
    bool success = queuePushBack(&sockets[sn].data_queue, new_segment);

    if (success) {
        memcpy(sockets[sn].data_buffer + write_index + 3, buf, len);
    }

    taskEXIT_CRITICAL();

    if (!success) {
        return SOCKERR_QUEUE_FULL;
    }
    return SOCK_OK;
}

// **DO NOT CALL FROM INTERRUPT CONTEXT!**
int sendto(uint8_t sn, uint8_t * buf, uint16_t len, uint8_t *addr, uint16_t port) {
    if (sn >= _WIZCHIP_SOCK_NUM_ || buf == NULL || len == 0 || addr == NULL || port == 0) {
        return SOCKERR_INVALID_PARAM;
    }

    if (sockets[sn].registers.SR != SOCK_UDP) {
        return SOCKERR_INVALID_STATE;
    }
    
    // Critical section
    // Ensure queue is not modified or DMA write isn't attempted until we finish
    taskENTER_CRITICAL();

    uint16_t write_index = getDataBufferIndex(&sockets[sn], len + 3);
    if (write_index == -1) {
        return SOCKERR_BUFFER_FULL;
    }

    // Add segment to queue
    BufferSegment new_segment;
    new_segment.start_index = write_index;
    new_segment.end_index = write_index + len + 3;
    memcpy(new_segment.addr, addr, 4);
    new_segment.port = port;
    bool success = queuePushBack(&sockets[sn].data_queue, new_segment);

    if (success) {
        memcpy(sockets[sn].data_buffer + write_index + 3, buf, len);
    }

    taskEXIT_CRITICAL();
    
    if (!success) {
        return SOCKERR_QUEUE_FULL;
    }
    return SOCK_OK;
}

static bool enqueueSetRegInline(uint32_t addr, const uint8_t* data, uint8_t len) {
    if (len > 6) {
        return false;
    }

    command_t cmd;
    // Add write bit and VDM operation mode
    addr |= (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_);
    
    cmd.is_addr = false;
    cmd.data.inline_buf[0] = (addr >> 16) & 0xFF;
    cmd.data.inline_buf[1] = (addr >> 8) & 0xFF;
    cmd.data.inline_buf[2] = addr & 0xFF;
    memcpy(&cmd.data.inline_buf[3], data, len);
    cmd.len = 3 + len;
    
    if (!queuePushBack(&command_queue, cmd)) {
        return false;
    }
    return true;
}

static bool enqueueGetRegInline(uint32_t addr, uint8_t* buffer, uint8_t len) {
    if (len > 6) {
        return false;
    }

    command_t cmd;
    // Add read bit and VDM operation mode
    addr |= (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_);
    
    cmd.is_addr = false;
    cmd.data.inline_buf[0] = (addr >> 16) & 0xFF;
    cmd.data.inline_buf[1] = (addr >> 8) & 0xFF;
    cmd.data.inline_buf[2] = addr & 0xFF;
    cmd.len = 3 + len;
    
    if (!queuePushBack(&command_queue, cmd)) {
        return false;
    }
    return true;
}

/**
 * @brief Poll a single byte register until it matches a value or timeout occurs
 * this is used for changes that **DO NOT** generate interrupts on the WIZNET
 * @return SOCK_OK on success, SOCKERR_TIMEOUT on timeout, SOCKERR_QUEUE_FULL on register read error
 */
static int pollRegNoIT(uint32_t addr, const uint8_t* reg, uint16_t val, bool inv=false, uint16_t timeout=2000) {
    uint16_t start_time = osKernelGetTickCount();
    while (osKernelGetTickCount() - start_time < timeout && (*reg != val) ^ inv) {
        if (!enqueueGetRegInline(addr, reg, 1)) {
            return SOCKERR_QUEUE_FULL;
        }
        osDelay(100);
    }
    if ((*reg == val) ^ inv) {
        return SOCK_OK;
    }
    return SOCKERR_TIMEOUT;
}

/**
 * @brief Poll a single byte register until it matches a value or timeout occurs
 * this is used for changes that **DO** generate interrupts on the WIZNET
 * @return SOCK_OK on success, SOCKERR_TIMEOUT on timeout
 */
static int pollRegWithIT(uint32_t addr, const uint8_t* reg, uint16_t val, bool inv=false) {
    uint16_t start_time = osKernelGetTickCount();
    // IT based polling should never timeout, but just in case communication is lost, we will timeout after 60 seconds
    while (osKernelGetTickCount() - start_time < 60000 && (*reg != val) ^ inv) {
        osDelay(100);
    }
    if ((*reg == val) ^ inv) {
        return SOCK_OK;
    }
    return SOCKERR_TIMEOUT;
}

static uint16_t getDataBufferIndex(socket_t* socket, uint16_t data_length)
{
    if (queueIsEmpty(&socket->data_queue)) {
        // Queue is empty - start at beginning if data fits
        return (data_length <= socket->data_buffer_size) ? 0 : -1;
    }
    
    // Get start and end positions from queue
    uint8_t* queue_start = queueFront(&socket->data_queue).start_index;
    uint8_t* queue_end = queueBack(&socket->data_queue).end_index;
    
    if (queue_end >= queue_start) {
        // End is after beginning: check space at end first, then at beginning
        uint16_t space_at_end = socket->data_buffer_size - queue_end;
        uint16_t space_at_beginning = queue_start;
        
        if (data_length <= space_at_end) {
            // Use space at end
            return queue_end;
        } else if (data_length <= space_at_beginning) {
            // Use space at beginning (wraparound)
            return 0;
        } else {
            // Not enough space
            return -1;
        }
    } else {
        // End is before beginning (already wrapped): only space between end and start
        if (data_length <= queue_start - queue_end) {
            return queue_end;
        } else {
            // Not enough space
            return -1;
        }
    }
}