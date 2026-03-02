# STM32 CAN Forwarder V2

## Overview

The STM32 CAN Forwarder is a high-performance data acquisition device built on the STM32H723VGT6 microcontroller. It captures CAN bus data from three simultaneous CAN interfaces, streams it to a remote server via WebSocket over Ethernet, and logs all data to an SD card for offline analysis.

### Key Features

- **Triple CAN Bus Interface**: Simultaneously monitors 3 independent CAN buses at different baud rates
- **Real-time Ethernet Streaming**: WebSocket-based data transmission with protobuf serialization
- **SD Card Logging**: Local data storage for CAN frames and system logs
- **Time Synchronization**: NTP client for accurate timestamping
- **DNS Resolution**: Dynamic server address resolution
- **ADC Monitoring**: Multi-channel analog sensor inputs (12 channels across 3 ADCs)
- **Watchdog Protection**: Independent watchdog timer for system reliability
- **RTOS Architecture**: FreeRTOS-based multithreaded design for concurrent operations

### Hardware Specifications

- **Microcontroller**: STM32H723VGT6 (ARM Cortex-M7 @ 550MHz)
- **Ethernet**: W5500 SPI Ethernet controller
- **Storage**: MicroSD card (SDMMC interface)
- **CAN**: 3x FDCAN peripherals (CAN 2.0B compatible)
- **ADC**: 3x 12-bit ADCs with 12 total channels
- **RTC**: Battery-backed real-time clock with LSE oscillator
- **Debug**: UART4 @ 115200 baud

---

## Software Architecture

The firmware is built using FreeRTOS and follows a modular, threaded architecture. Each major function runs in its own task with message queues for inter-task communication.

### FreeRTOS Tasks

1. **Default Task** (`StartDefaultTask`): System initialization and startup
2. **Watchdog Task** (`watchdogHandler`): Independent watchdog refresh
3. **App Layer Task** (`appLayerThread`): WebSocket communication and streaming
4. **Storage Task** (`storageThread`): SD card data logging
5. **DNS Resolve Task** (`dnsResolveThread`): DNS query handling
6. **UNIX Time Task**: NTP synchronization and timekeeping
7. **Stats Task**: System statistics monitoring and logging

### Message Queues

- `storageCanQueue`: CAN frames for SD card logging (128 entries)
- `canStreamQueue`: CAN frames for WebSocket streaming (256 entries)
- `dnsReqQueue`: DNS resolution requests (4 entries)
- `logQueue`: System log messages for UART output (8 entries)
- `storageLogQueue`: System log messages for SD card logging (8 entries)

---

## Software Components

### Core Application Files

Located in `Core/Src/` and `Core/Inc/`:

#### `can_driver.c`

**Purpose**: CAN bus management and data acquisition

**Key Functions**:

- `initCAN()`: Configures all three FDCAN peripherals with filters and interrupts
- `drainFifoToQueue()`: Reads CAN messages from hardware FIFO and queues them
- `HAL_FDCAN_RxFifo0Callback()`: Interrupt handler triggered on CAN message reception

**Features**:

- Accepts all standard (11-bit) and extended (29-bit) CAN IDs
- Hardware timestamping with microsecond precision
- Zero-copy message handling using FIFOs
- Automatic message routing to storage and streaming queues
- Bus-specific timestamp synchronization with UNIX time

**Data Flow**:

```
CAN Bus → FDCAN RX FIFO → drainFifoToQueue() → canStreamQueue → WebSocket
                                              ↘ storageCanQueue → SD Card
```

---

#### `app_layer.c`

**Purpose**: WebSocket client for real-time data streaming

**Key Functions**:

- `initAppLayer()`: Creates app layer task
- `appLayerThread()`: Main loop for WebSocket connection management
- `packetEncode()`: Serializes CAN frames using nanopb protobuf

**Operation**:

1. Requests DNS resolution for WebSocket server hostname
2. Waits for NTP time synchronization
3. Establishes WebSocket connection to remote server
4. Collects CAN frames from `canStreamQueue` (up to 9 per packet)
5. Encodes frames as protobuf binary message
6. Sends via WebSocket binary frame
7. Monitors connection health and reconnects if needed

**Configuration**:

- Server: Configured via `WS_DOMAIN_NAME` and `WS_DOMAIN_PATH` macros
- Port: 80 (HTTP WebSocket)
- Buffer sizes: 4KB TX, 1KB RX
- Protocol: Binary WebSocket with protobuf payload

---

#### `storage.c`

**Purpose**: SD card data logging

**Key Functions**:

- `initStorage()`: Creates timestamped filenames and starts storage task
- `storageThread()`: Main loop for SD card operations

**Features**:

- Automatic SD card detection (hardware card detect pin)
- Separate files for CAN data (binary) and system logs (text)
- Filename format: `YYYY-MM-DD_HH-MM-SS_can.bin` and `YYYY-MM-DD_HH-MM-SS_log.txt`
- Periodic `f_sync()` every 10 seconds to prevent data loss
- Batch writes for efficiency (5 CAN frames per write)
- Throughput monitoring and statistics

**File Formats**:

- CAN file: Raw binary `CanFrame` structures (protobuf-compatible)
- Log file: Plain text with timestamps

---

#### `dns_resolve.c`

**Purpose**: DNS resolution service

**Key Functions**:

- `initDNSResolve()`: Initializes DNS client and creates task
- `dnsResolveThread()`: Processes DNS resolution requests from queue

**Configuration**:

- DNS Server: 8.8.8.8 (Google Public DNS)
- Timeout: 5 seconds
- Retries: 3 attempts
- Socket: Dedicated W5500 socket for DNS

**Usage**:
Applications submit DNS requests via `dnsReqQueue` with a callback function. Results (IP address and TTL) are returned asynchronously.

---

#### `unix_time.c`

**Purpose**: Network time synchronization

**Key Functions**:

- `initTime()`: Starts NTP client and time tracking task
- `UNIX_Timer_Callback()`: TIM2 interrupt handler for microsecond counter
- `getCurrentUnixTime()`: Returns current UNIX timestamp

**Features**:

- NTP client for internet time synchronization
- Hardware timer (TIM2) for microsecond resolution
- Automatic RTC synchronization on NTP success
- Global timestamp for CAN message timestamping

**NTP Configuration**:

- Server: `pool.ntp.org`
- Update interval: Periodic (configurable)

---

#### `w5500_setup.c`

**Purpose**: Ethernet controller initialization

**Key Functions**:

- `W5500Init()`: Initializes W5500 chip via SPI
- `initWizchip()`: Configures network parameters and socket buffers

**Network Configuration**:

- IP Address: 10.0.0.69 (static)
- Gateway: 10.0.0.1
- Subnet Mask: 255.255.255.0
- MAC Address: Generated or hardcoded

**Socket Allocation**:

- Socket 0: WebSocket streaming (16KB TX buffer)
- Socket 1: DNS client (2KB buffers)
- Socket 2: NTP client (2KB buffers)

---

#### `adc_scanner.c`

**Purpose**: Analog sensor monitoring

**Key Functions**:

- `ADC_Scanner_Init()`: Configures TIM7 for periodic ADC triggers
- `ADC_Scanner_TriggerConversions()`: Starts DMA-based ADC conversions
- `ADC_Scanner_GetValue()`: Returns latest ADC reading for a channel

**Features**:

- 12 channels across 3 ADC peripherals
- DMA-based data transfer (no CPU overhead)
- Configurable sampling rate via TIM7
- 12-bit resolution (0-4095)

**Channel Mapping**:

- ADC1: Channels 16, 17, 18, 19, 3 (5 channels)
- ADC2: Channels 14, 15, 7, 4, 8 (5 channels)
- ADC3: Channels 0, 1 (2 channels)

---

#### `log_handler.c`

**Purpose**: System logging infrastructure

**Key Functions**:

- `log_init()`: Initializes UART and log task
- `log_msg()`: Queues log message with level and formatting
- `logHandlerThread()`: Outputs logs to UART and SD card

**Log Levels**:

- `LL_DBG`: Debug messages
- `LL_WRN`: Warnings
- `LL_ERR`: Errors

**Output**:

- UART4: Real-time log streaming for debugging
- SD Card: Persistent log storage (via `storageLogQueue`)

---

#### `stats.c`

**Purpose**: System performance monitoring

**Key Functions**:

- `initStats()`: Creates statistics monitoring task
- `statsThread()`: Periodically reports system metrics

**Metrics**:

- CAN packets processed
- Dropped packets
- Queue utilization
- Ethernet throughput
- SD card write performance

---

### Supporting Files

#### `forwarder_pb.pb.c` / `forwarder_pb.pb.h`

Auto-generated protobuf message definitions for CAN frame serialization.

**Message Structure** (`Packet`):

```protobuf
message CanFrame {
    uint32 can_id = 1;
    uint32 can_bus = 2;
    uint64 timestamp = 3;
    bytes can_data = 4;
}

message Packet {
    repeated CanFrame canFrames = 1;
}
```

---

## Library Dependencies

### 1. Ethernet Library (`Lib/Ethernet/`)

A comprehensive Ethernet communication library for the W5500 chip.

See more details at: [https://github.com/killer400033/wiznet-driver](https://github.com/killer400033/wiznet-driver)

---

### 2. Nanopb Library (`Lib/Nanopb/`)

A lightweight Protocol Buffers implementation designed for embedded systems.

Taken from: [https://github.com/nanopb/nanopb](https://github.com/nanopb/nanopb)

---

### 3. DBCGen Library (`Lib/DBCGen/`)

A DBC (CAN Database) file parser for interpreting CAN messages.

See more details at: [https://github.com/killer400033/dbcGenPy](https://github.com/killer400033/dbcGenPy)

---

## Hardware Section

![PCB View](/img/pcb.jpg)

![3D PCB Front](/img/pcb-front.jpg)

![3D PCB Back](/img/pcb-back.jpg)