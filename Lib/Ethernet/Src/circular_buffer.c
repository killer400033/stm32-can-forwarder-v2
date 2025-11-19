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