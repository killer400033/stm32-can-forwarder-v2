#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include "socket.h"
#include <stdint.h>
#include <stdbool.h>

// WebSocket constants
#define WS_MAGIC_STRING "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define WS_KEY_LEN 16
#define WS_ACCEPT_LEN 28
#define WS_MAX_HEADER_LEN 14
#define WS_MAX_PAYLOAD_LEN 1024

// WebSocket opcodes
typedef enum {
    WS_OPCODE_CONTINUATION = 0x0,
    WS_OPCODE_TEXT = 0x1,
    WS_OPCODE_BINARY = 0x2,
    WS_OPCODE_CLOSE = 0x8,
    WS_OPCODE_PING = 0x9,
    WS_OPCODE_PONG = 0xA
} ws_opcode_t;

// WebSocket frame structure
typedef struct {
    bool fin;
    bool mask;
    ws_opcode_t opcode;
    uint16_t payload_len;
    uint8_t masking_key[4];
    uint8_t *payload;
} ws_frame_t;

// WebSocket client state
typedef enum {
    WS_STATE_DISCONNECTED,
    WS_STATE_CONNECTING,
    WS_STATE_HANDSHAKE,
    WS_STATE_CONNECTED,
    WS_STATE_CLOSING,
    WS_STATE_ERROR
} ws_state_t;

// WebSocket client structure
typedef struct {
    uint8_t socket_num;
    ws_state_t state;
    char host[64];
    uint16_t port;
    char path[128];
    char sec_websocket_key[25];
    uint8_t rx_buffer[WS_MAX_PAYLOAD_LEN + WS_MAX_HEADER_LEN];
    uint8_t tx_buffer[WS_MAX_PAYLOAD_LEN + WS_MAX_HEADER_LEN];
} websocket_client_t;

// Function prototypes
int8_t ws_client_init(websocket_client_t *client, uint8_t socket_num);
int8_t ws_client_connect(websocket_client_t *client, const char *host, uint16_t port, const char *path);
int8_t ws_client_send_text(websocket_client_t *client, const char *text);
int8_t ws_client_send_binary(websocket_client_t *client, const uint8_t *data, uint16_t len);
int8_t ws_client_receive(websocket_client_t *client, uint8_t *buffer, uint16_t *len, ws_opcode_t *opcode);
int8_t ws_client_ping(websocket_client_t *client);
int8_t ws_client_close(websocket_client_t *client);
void ws_client_process(websocket_client_t *client);

// Helper functions
static void ws_generate_key(char *key);
static void ws_base64_encode(const uint8_t *input, uint16_t input_len, char *output);
static int8_t ws_parse_handshake_response(websocket_client_t *client, const char *response);
static uint16_t ws_create_frame(uint8_t *buffer, ws_opcode_t opcode, const uint8_t *payload, uint16_t payload_len, bool mask);
static int8_t ws_parse_frame(const uint8_t *buffer, uint16_t buffer_len, ws_frame_t *frame);
static void ws_mask_payload(uint8_t *payload, uint16_t len, const uint8_t *mask);

#endif // WEBSOCKET_CLIENT_H
