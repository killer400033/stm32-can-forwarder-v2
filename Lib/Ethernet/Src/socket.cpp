#include <queue>
#include <cstring>
#include "socket.hpp"
#include "w5500.h"
#include "cmsis_os.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

static std::queue<command_t> command_queue;

socket_t sockets[_WIZCHIP_SOCK_NUM_];

static void enqueueCmdInline(uint32_t addr, const uint8_t* data, uint8_t len) {
    command_t cmd;
    // Add write bit and VDM operation mode
    addr |= (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_);
    
    cmd.is_addr = false;
    cmd.data.inline_buf[0] = (addr >> 16) & 0xFF;
    cmd.data.inline_buf[1] = (addr >> 8) & 0xFF;
    cmd.data.inline_buf[2] = addr & 0xFF;
    memcpy(&cmd.data.inline_buf[3], data, len);
    cmd.len = 3 + len;
    
    command_queue.push(cmd);
}

void initWizchip(uint8_t* ip_address, uint8_t* subnet_mask, uint8_t* gateway_ip) {
    const uint8_t mac_addr[] = MAC_ADDRESS;
    const uint16_t intlevel = INTLEVEL;
    const uint16_t rtr = RETRY_TIME * 10;
    const uint8_t rcr = RETRY_COUNT;
    
    resetWizchip();
    
    // Enqueue commands to write common registers
    enqueueCmdInline(GAR, gateway_ip, 4);
    enqueueCmdInline(SUBR, subnet_mask, 4);
    enqueueCmdInline(SHAR, mac_addr, 6);
    enqueueCmdInline(SIPR, ip_address, 4);
    
    // Write INTLEVEL
    uint8_t intlevel_bytes[2];
    intlevel_bytes[0] = (intlevel >> 8) & 0xFF;
    intlevel_bytes[1] = intlevel & 0xFF;
    enqueueCmdInline(INTLEVEL, intlevel_bytes, 2);
    
    // Write RTR
    uint8_t rtr_bytes[2];
    rtr_bytes[0] = (rtr >> 8) & 0xFF;
    rtr_bytes[1] = rtr & 0xFF;
    enqueueCmdInline(W5500_RTR, rtr_bytes, 2);
    
    // Write RCR
    enqueueCmdInline(W5500_RCR, &rcr, 1);
}


void initSocket(uint8_t socket_number, uint8_t* data_buffer, uint16_t data_buffer_size) {
    sockets[socket_number].data_buffer = data_buffer;
    sockets[socket_number].data_buffer_size = data_buffer_size;
    sockets[socket_number].is_sending = false;
    sockets[socket_number].reg_a = {0};
    sockets[socket_number].reg_b = {0};
    sockets[socket_number].reg_c = {0};
    sockets[socket_number].reg_d = {0};

    sockets[socket_number].reg_d.KPALVTR = KEEP_ALIVE_TIMER / 5; // Convert seconds to 5s units
}

/**
 * @brief Add data to the buffer
 */
bool enqueueData(const uint8_t* data, uint16_t length) {
    uint16_t write_index = getDataBufferIndex(length);
    if (write_index == -1) {
        return false;
    }

    memcpy(buffer + write_index, data, length);

    // Add segment to queue
    BufferSegment new_segment;
    new_segment.start_index = write_index;
    new_segment.end_index = write_index + length;
    sockets[socket_number].data_queue.push(new_segment);

    return true;
}

static uint16_t getDataBufferIndex(uint16_t data_length)
{
    if (queue.empty()) {
        // Queue is empty - start at beginning if data fits
        return (data_length <= SOCKET_BUFFER_SIZE) ? 0 : -1;
    }
    
    // Get start and end positions from queue
    uint8_t* queue_start = queue.front().start_index;
    uint8_t* queue_end = queue.back().end_index;
    
    if (queue_end >= queue_start) {
        // End is after beginning: check space at end first, then at beginning
        uint16_t space_at_end = SOCKET_BUFFER_SIZE - queue_end;
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

static void resetWizchip(void) {    
    if (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
        HAL_SPI_Abort(&hspi1);
        osDelay(10);
    }
    
    uint8_t reset_cmd[4];
    uint32_t addr = MR | (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_);
    reset_cmd[0] = (addr >> 16) & 0xFF;  // Address high byte
    reset_cmd[1] = (addr >> 8) & 0xFF;   // Address mid byte
    reset_cmd[2] = addr & 0xFF;          // Address low byte with control bits
    reset_cmd[3] = MR_RST;               // Reset bit (0x80)
    
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, reset_cmd, sizeof(reset_cmd), 1000);
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
    
    // Wait for W5500 to complete reset
    osDelay(100);
}