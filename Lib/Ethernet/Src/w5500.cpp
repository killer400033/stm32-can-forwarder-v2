#include <cstring>
#include "w5500.hpp"
#include "socket.hpp"
#include "cmsis_os.h"
#include "main.h"

static Queue<command_t, COMMAND_QUEUE_SIZE> command_queue;
command_t running_cmd = {0};
common_regs_t common_regs = {0};

static SPI_HandleTypeDef* wiznet_hspi = NULL;

/**
 * @brief Initialize W5500 chip with network configuration
 * @warning **MUST NOT be called from interrupt context!** This function uses blocking operations, osDelay(), and taskENTER_CRITICAL().
 * @param ip_address IP address (4 bytes)
 * @param subnet_mask Subnet mask (4 bytes)
 * @param gateway_ip Gateway IP address (4 bytes)
 * @param rx_buf_sizes RX buffer sizes for all 8 sockets (in KB)
 * @param tx_buf_sizes TX buffer sizes for all 8 sockets (in KB)
 * @param hspi SPI handle for W5500 communication
 * @return SOCK_OK (0) on success, negative error code otherwise
 */
int initWizchip(uint8_t* ip_address, uint8_t* subnet_mask, uint8_t* gateway_ip,
                const uint8_t* rx_buf_sizes, const uint8_t* tx_buf_sizes, SPI_HandleTypeDef* hspi) {
    if (hspi == NULL) {
        return SOCKERR_INVALID_PARAM;
    }

    wiznet_hspi = hspi;

    const uint8_t mac_addr[] = MAC_ADDRESS;
    const uint16_t intlevel = INTLEVEL;
    const uint16_t rtr = RETRY_TIME * 10;
    const uint8_t rcr = RETRY_COUNT;

    queueInit(&command_queue);

    // Write reset command
    common_regs.MR = MR_RST;
    if (!enqueueSetReg(0xFF, MR, &common_regs.MR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Poll for reset to complete
    int ret = pollRegNoIT(0xFF, MR, &common_regs.MR, 0x00);
    if (ret != SOCK_OK) {
        return ret;
    }
    
    // Write Network Information
    common_regs.GAR = gateway_ip;
    if (!enqueueSetReg(0xFF, GAR, common_regs.GAR, 4)) {
        return SOCKERR_QUEUE_FULL;
    }
    common_regs.SUBR = subnet_mask;
    if (!enqueueSetReg(0xFF, SUBR, common_regs.SUBR, 4)) {
        return SOCKERR_QUEUE_FULL;
    }
    common_regs.SHAR = mac_addr;
    if (!enqueueSetReg(0xFF, SHAR, common_regs.SHAR, 6)) {
        return SOCKERR_QUEUE_FULL;
    }
    common_regs.SIPR = ip_address;
    if (!enqueueSetReg(0xFF, SIPR, common_regs.SIPR, 4)) {
        return SOCKERR_QUEUE_FULL;
    }
    
    // Write INTLEVEL
    common_regs.INTLEVEL[0] = (intlevel >> 8) & 0xFF;
    common_regs.INTLEVEL[1] = intlevel & 0xFF;
    if (!enqueueSetReg(0xFF, INTLEVEL, common_regs.INTLEVEL, 2)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Enable interrupts for all sockets
    common_regs.SIMR = 0xFF;
    if (!enqueueSetReg(0xFF, SIMR, &common_regs.SIMR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Disable all common interrupts
    common_regs.IMR = 0x00;
    if (!enqueueSetReg(0xFF, IMR, &common_regs.IMR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }
    
    // Write RTR
    common_regs.RTR[0] = (rtr >> 8) & 0xFF;
    common_regs.RTR[1] = rtr & 0xFF;
    if (!enqueueSetReg(0xFF, W5500_RTR, common_regs.RTR, 2)) {
        return SOCKERR_QUEUE_FULL;
    }
    
    // Write RCR
    common_regs.RCR = rcr;
    if (!enqueueSetReg(0xFF, W5500_RCR, common_regs.RCR, 1)) {
        return SOCKERR_QUEUE_FULL;
    }

    // Write buffer sizes for all 8 sockets
    for (uint8_t i = 0; i < _WIZCHIP_SOCK_NUM_; i++) {
        // Write Sn_RXBUF_SIZE (RX Buffer Size) - 1 byte
        sockets[i].registers.RXBUF_SIZE = rx_buf_sizes[i];
        if (!enqueueSetReg(i, Sn_RXBUF_SIZE(i), &sockets[i].registers.RXBUF_SIZE, 1)) {
            return SOCKERR_QUEUE_FULL;
        }
        
        // Write Sn_TXBUF_SIZE (TX Buffer Size) - 1 byte
        sockets[i].registers.TXBUF_SIZE = tx_buf_sizes[i];
        if (!enqueueSetReg(i, Sn_TXBUF_SIZE(i), &sockets[i].registers.TXBUF_SIZE, 1)) {
            return SOCKERR_QUEUE_FULL;
        }
    }

    return SOCK_OK;
}

// **DO NOT CALL FROM INTERRUPT CONTEXT!**
bool enqueueSetReg(uint8_t sn, uint32_t addr, const uint8_t* data, uint8_t len) {
    if (len > COMMAND_BUFFER_SIZE - 3) {
        return false;
    }

    command_t cmd;
    generateSetRegCmd(&cmd, sn, addr, data, len);
    
    taskENTER_CRITICAL();
    bool success = queuePushBack(&command_queue, cmd);
    taskEXIT_CRITICAL();

    return success;
}

// **DO NOT CALL FROM INTERRUPT CONTEXT!**
bool enqueueGetReg(uint8_t sn, uint32_t addr, uint8_t* buffer, uint8_t len) {
    if (len > COMMAND_BUFFER_SIZE - 3) {
        return false;
    }

    command_t cmd;
    generateGetRegCmd(&cmd, sn, addr, buffer, len);
    
    taskENTER_CRITICAL();
    bool success = queuePushBack(&command_queue, cmd);
    taskEXIT_CRITICAL();

    return success;
}

/**
 * @brief Poll a single byte register until it matches a value or timeout occurs
 * this is used for changes that **DO NOT** generate interrupts on the WIZNET
 * @return SOCK_OK on success, SOCKERR_TIMEOUT on timeout, SOCKERR_QUEUE_FULL on register read error
 */
int pollRegNoIT(uint8_t sn, uint32_t addr, const uint8_t* reg, uint16_t val, bool inv=false, uint16_t timeout=2000) {
    uint16_t start_time = osKernelGetTickCount();
    while (osKernelGetTickCount() - start_time < timeout && (*reg != val) ^ inv) {
        if (!enqueueGetReg(sn, addr, reg, 1)) {
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
int pollRegWithIT(uint32_t addr, const uint8_t* reg, uint16_t val, bool inv=false) {
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

/**
 * DMA transmit/receive complete callback
 * Called when a DMA transmit/receive operation is completed
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == wiznet_hspi->Instance) {
        // Process the received data first
        dmaRXCompleteCallback();
        
        // Then handle the TX completion (queue management)
        dmaTXCompleteCallback();
    }
}

/**
 * DMA transmit complete callback
 * Called when a DMA transmission is completed
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == wiznet_hspi->Instance) {
        dmaTXCompleteCallback();
    }
}

/**
 * @brief Get the index of the TX buffer where the next data should be written
 * @param socket The socket to get the index for
 * @param data_length The length of the data to write
 * @return The index of the data buffer where the next data should be written, -1 if there is not enough space
 */
int16_t getTXBufferIndex(socket_t* socket, uint16_t len)
{
    if (queueIsEmpty(&socket->tx_buf_queue)) {
        // Queue is empty - start at beginning if data fits
        return (len <= socket->tx_buf_len) ? 0 : -1;
    }
    
    // Get start and end positions from queue
    int16_t queue_start = queueFront(&socket->tx_buf_queue).start_index;
    int16_t queue_end = queueBack(&socket->tx_buf_queue).end_index;
    
    if (queue_end >= queue_start) {
        // End is after beginning: check space at end first, then at beginning
        int16_t space_at_end = socket->tx_buf_len - queue_end;
        int16_t space_at_beginning = queue_start;
        
        if (len <= space_at_end) {
            // Use space at end
            return queue_end;
        } else if (len <= space_at_beginning) {
            // Use space at beginning (wraparound)
            return 0;
        } else {
            // Not enough space
            return -1;
        }
    } else {
        // End is before beginning (already wrapped): only space between end and start
        if (len <= queue_start - queue_end) {
            return queue_end;
        } else {
            // Not enough space
            return -1;
        }
    }
}

/**
 * @brief Get the index of the RX buffer where the next data should be written
 * @param socket The socket to get the index for
 * @param len The length of the data to write
 * @return The index of the RX buffer where the next data should be written, -1 if there is not enough space
 */
int16_t getRXBufferIndex(socket_t* socket, uint16_t len) {
    int16_t queue_start = socket->rx_buf_start_idx;
    int16_t queue_end = socket->rx_buf_end_idx;

    if (queue_end >= queue_start) {
        // End is after beginning: check space at end first, then at beginning
        int16_t space_at_end = socket->rx_buf_len - queue_end;
        int16_t space_at_beginning = queue_start;
        
        if (len <= space_at_end) {
            // Use space at end
            return queue_end;
        } else if (len <= space_at_beginning) {
            // Use space at beginning (wraparound)
            return 0;
        } else {
            // Not enough space
            return -1;
        }
    } else {
        // End is before beginning (already wrapped): only space between end and start
        if (len <= queue_start - queue_end) {
            return queue_end;
        } else {
            // Not enough space
            return -1;
        }
    }
}

static void dmaTXCompleteCallback(void) {
    // Ensure only we access queue
    uint32_t isrm = taskENTER_CRITICAL_FROM_ISR();

    bool success = queuePopFront(&command_queue, &running_cmd);
    
    taskEXIT_CRITICAL_FROM_ISR(isrm);

    if (!success) return;

    switch (running_cmd.cmd_type) {
        case WRITE_REG:
            // Check if this is a Sn_CR (Command Register) write
            if (running_cmd.inline_buf[0] == 0x00 && running_cmd.inline_buf[1] == 0x01 && ((running_cmd.inline_buf[2] >> 2) & 0b111) == 0b011) {
                if (running_cmd.sn < _WIZCHIP_SOCK_NUM_) {
                    command_t read_cmd;
                    generateGetRegCmd(&read_cmd, running_cmd.sn, Sn_CR(running_cmd.sn), &sockets[running_cmd.sn].registers.CR, 1);
                    
                    if (sockets[running_cmd.sn].registers.CR != 0x00) {
                        // CR register is busy, send read command to get latest CR value instead
                        HAL_SPI_TransmitReceive_DMA(wiznet_hspi, read_cmd.inline_buf, read_cmd.inline_buf, read_cmd.len);
                        
                        // Re-enqueue the original write command to front
                        uint32_t isrm = taskENTER_CRITICAL_FROM_ISR();
                        queuePushFront(&command_queue, running_cmd);
                        taskEXIT_CRITICAL_FROM_ISR(isrm);
                    }
                    else {
                        // CR register is free, send write command
                        HAL_SPI_Transmit_DMA(wiznet_hspi, running_cmd.inline_buf, running_cmd.len);

                        // Enqueue read command to front to update CR as soon as possible
                        uint32_t isrm = taskENTER_CRITICAL_FROM_ISR();
                        queuePushFront(&command_queue, read_cmd);
                        taskEXIT_CRITICAL_FROM_ISR(isrm);
                    }
                }
            }
            else {
                // Normal write command, just send it
                HAL_SPI_Transmit_DMA(wiznet_hspi, running_cmd.inline_buf, running_cmd.len);
            }
            break;
        case WRITE_BUF:
            HAL_SPI_Transmit_DMA(wiznet_hspi, running_cmd.ptr, running_cmd.len);
            break;
        case READ_REG:
            HAL_SPI_TransmitReceive_DMA(wiznet_hspi, running_cmd.inline_buf, running_cmd.inline_buf, running_cmd.len);
            break;
        case READ_BUF:
            HAL_SPI_TransmitReceive_DMA(wiznet_hspi, running_cmd.ptr, running_cmd.ptr, running_cmd.len);
            break;
        case READ_SIR:
            HAL_SPI_TransmitReceive_DMA(wiznet_hspi, running_cmd.inline_buf, running_cmd.inline_buf, running_cmd.len);
            break;
        default:
            break;
    }
}

static void dmaRXCompleteCallback(void) {
    socket_t* socket = &sockets[running_cmd.sn];
    switch (running_cmd.cmd_type) {
        case READ_REG:
            memcpy(running_cmd.ptr, &(running_cmd.inline_buf[3]), running_cmd.len - 3);
            break;
        case READ_BUF:
            // Data put into buffer, inform upper layer
            if (socket->callback != NULL) {
                uint16_t len = running_cmd.len - 3;
                socket->callback(SOCKET_RECV_CALLBACK, &len);
            }
            break;
        // Called when interrupt is generated, and we read SIR register
        case READ_SIR:
            memcpy(running_cmd.ptr, &(running_cmd.inline_buf[3]), running_cmd.len - 3);
            command_t cmd;
            for (uint8_t i = 0; i < _WIZCHIP_SOCK_NUM_; i++) {
                if ((common_regs.SIR >> i) & 0x01) {
                    generateGetBufCmd(&cmd, i, Sn_MR(i), (uint8_t*)&(sockets[i].registers), sizeof(sockets[i].registers));
                    cmd.cmd_type = READ_SOC;
                    
                    uint32_t isrm = taskENTER_CRITICAL_FROM_ISR();
                    queuePushFront(&command_queue, cmd);
                    taskEXIT_CRITICAL_FROM_ISR(isrm);
                }
            }
            break;
        // Called when interrupt is generated, and we read socket regs for sockets that have interrupts
        case READ_SOC:
            if (socket->registers.IR & Sn_IR_CON) {
                // Connection established
                // No action needed
            }

            if (socket->registers.IR & Sn_IR_DISCON) {
                if (socket->registers.SR == SOCK_CLOSE_WAIT) {
                    // Disconnect request from peer, inform upper layer
                    if (socket->callback != NULL) {
                        socket->callback(SOCKET_DISCON_REQ_CALLBACK, NULL);
                    }
                }
                else {
                    // Disconnect request from us successful
                    if (socket->callback != NULL) {
                        socket->callback(SOCKET_DISCON_SUC_CALLBACK, NULL);
                    }
                }
            }

            if (socket->registers.IR & Sn_IR_RECV) {
                // Data received
                uint32_t isrm = taskENTER_CRITICAL_FROM_ISR();

                uint32_t rx_buf_index = getRXBufferIndex(socket, socket->registers.RX_RSR + 3);
                if (rx_buf_index != -1) {
                    command_t cmd;
                    uint32_t ptr = socket->registers.RX_RD;
                    uint32_t addr = ((uint32_t)ptr << 8) + (WIZCHIP_RXBUF_BLOCK(sn) << 3);
                    generateGetBufCmd(&cmd, running_cmd.sn, addr, socket->rx_buf + rx_buf_index, socket->registers.RX_RSR + 3);

                    queuePushFront(&command_queue, cmd);
                }


                taskEXIT_CRITICAL_FROM_ISR(isrm);
            }

            if (socket->registers.IR & Sn_IR_TIMEOUT) {
                // Timeout occurred, socket is closed
                if (socket->callback != NULL) {
                    socket->callback(SOCKET_TIMEOUT_CALLBACK, NULL);
                }
            }
            
            if (socket->registers.IR & Sn_IR_SENDOK) {
                // Send OK, socket is ready to send data
            }

            // Clear the interrupt register by writing the same value back
            command_t cmd;
            generateSetRegCmd(&cmd, running_cmd.sn, Sn_IR(running_cmd.sn), &sockets[running_cmd.sn].registers.IR, 1);
            
            uint32_t isrm = taskENTER_CRITICAL_FROM_ISR();
            queuePushFront(&command_queue, cmd);
            taskEXIT_CRITICAL_FROM_ISR(isrm);

            break;
        default:
            break;
    }
}

static inline void generateSetRegCmd(command_t* cmd, uint8_t sn, uint32_t addr, const uint8_t* data, uint8_t len) {
    addr |= (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_);

    cmd->cmd_type = WRITE_REG;
    cmd->sn = sn;
    cmd->inline_buf[0] = (addr >> 16) & 0xFF;
    cmd->inline_buf[1] = (addr >> 8) & 0xFF;
    cmd->inline_buf[2] = addr & 0xFF;
    memcpy(&cmd->inline_buf[3], data, len);
    cmd->len = 3 + len;
}

static inline void generateGetRegCmd(command_t* cmd, uint8_t sn, uint32_t addr, uint8_t* buffer, uint8_t len) {
    addr |= (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_);

    cmd->cmd_type = READ_REG;
    cmd->sn = sn;
    cmd->ptr = buffer;
    cmd->inline_buf[0] = (addr >> 16) & 0xFF;
    cmd->inline_buf[1] = (addr >> 8) & 0xFF;
    cmd->inline_buf[2] = addr & 0xFF;
    cmd->len = 3 + len;
}

static inline void generateSetBufCmd(command_t* cmd, uint8_t sn, uint32_t addr, uint8_t* buffer, uint8_t len) {
    addr |= (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_);

    cmd->cmd_type = WRITE_BUF;
    cmd->sn = sn;
    cmd->ptr = buffer;
    cmd->ptr[0] = (addr >> 16) & 0xFF;
    cmd->ptr[1] = (addr >> 8) & 0xFF;
    cmd->ptr[2] = addr & 0xFF;
    cmd->len = len;
}

static inline void generateGetBufCmd(command_t* cmd, uint8_t sn, uint32_t addr, uint8_t* buffer, uint8_t len) {
    addr |= (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_);

    cmd->cmd_type = READ_BUF;
    cmd->sn = sn;
    cmd->ptr = buffer;
    cmd->ptr[0] = (addr >> 16) & 0xFF;
    cmd->ptr[1] = (addr >> 8) & 0xFF;
    cmd->ptr[2] = addr & 0xFF;
    cmd->len = len;
}