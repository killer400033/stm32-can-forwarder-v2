#include "websocket_client.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Base64 encoding table
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Simple pseudo-random number generator for masking keys
static uint32_t ws_rand_seed = 1;
static uint32_t ws_rand(void) {
    ws_rand_seed = ws_rand_seed * 1103515245 + 12345;
    return ws_rand_seed;
}

// Initialize WebSocket client
int8_t ws_client_init(websocket_client_t *client, uint8_t socket_num) {
    if (!client || socket_num >= _WIZCHIP_SOCK_NUM_) {
        return -1;
    }

    memset(client, 0, sizeof(websocket_client_t));
    client->socket_num = socket_num;
    client->state = WS_STATE_DISCONNECTED;

    return 0;
}

// Connect to WebSocket server
int8_t ws_client_connect(websocket_client_t *client, const char *host, uint16_t port, const char *path) {
    if (!client || !host || !path) {
        return -1;
    }

    // Store connection parameters
    strncpy(client->host, host, sizeof(client->host) - 1);
    client->port = port;
    strncpy(client->path, path, sizeof(client->path) - 1);

    // Resolve hostname to IP (simplified - assuming IP address is provided)
    uint8_t server_ip[4];
    if (sscanf(host, "%u.%u.%u.%u", &server_ip[0], &server_ip[1], &server_ip[2], &server_ip[3]) != 4) {
        client->state = WS_STATE_ERROR;
        return -1;
    }

    // Create TCP socket
    int8_t result = socket(client->socket_num, Sn_MR_TCP, 0, 0);
    if (result != client->socket_num) {
        client->state = WS_STATE_ERROR;
        printf("2\n");
        return -1;
    }

    // Connect to server
    client->state = WS_STATE_CONNECTING;
    result = connect(client->socket_num, server_ip, port);
    if (result != SOCK_OK) {
        client->state = WS_STATE_ERROR;
        close(client->socket_num);
        printf("3\n");
        return -1;
    }

    // Generate WebSocket key
    ws_generate_key(client->sec_websocket_key);

    // Send WebSocket handshake
    char handshake[512];
    snprintf(handshake, sizeof(handshake),
        "GET %s HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: %s\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "\r\n",
        path, host, port, client->sec_websocket_key);

    int32_t sent = send(client->socket_num, (uint8_t*)handshake, strlen(handshake));
    if (sent <= 0) {
        client->state = WS_STATE_ERROR;
        close(client->socket_num);
        return -1;
    }

    client->state = WS_STATE_HANDSHAKE;
    return 0;
}

// Generate random WebSocket key
static void ws_generate_key(char *key) {
    uint8_t raw_key[WS_KEY_LEN];

    // Generate 16 random bytes
    for (int i = 0; i < WS_KEY_LEN; i++) {
        raw_key[i] = (uint8_t)(ws_rand() & 0xFF);
    }

    // Base64 encode
    ws_base64_encode(raw_key, WS_KEY_LEN, key);
    key[24] = '\0'; // Null terminate
}

// Base64 encoding
static void ws_base64_encode(const uint8_t *input, uint16_t input_len, char *output) {
    uint16_t i, j;

    for (i = 0, j = 0; i < input_len; i += 3, j += 4) {
        uint32_t octet_a = i < input_len ? input[i] : 0;
        uint32_t octet_b = i + 1 < input_len ? input[i + 1] : 0;
        uint32_t octet_c = i + 2 < input_len ? input[i + 2] : 0;

        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        output[j] = base64_table[(triple >> 18) & 0x3F];
        output[j + 1] = base64_table[(triple >> 12) & 0x3F];
        output[j + 2] = i + 1 < input_len ? base64_table[(triple >> 6) & 0x3F] : '=';
        output[j + 3] = i + 2 < input_len ? base64_table[triple & 0x3F] : '=';
    }
}

// Parse handshake response
static int8_t ws_parse_handshake_response(websocket_client_t *client, const char *response) {
    // Simple check for "101 Switching Protocols"
    if (strstr(response, "101") == NULL || strstr(response, "Switching Protocols") == NULL) {
        return -1;
    }

    // Should also verify Sec-WebSocket-Accept header, but simplified for this example
    return 0;
}

// Send text frame
int8_t ws_client_send_text(websocket_client_t *client, const char *text) {
    if (!client || !text || client->state != WS_STATE_CONNECTED) {
        return -1;
    }

    uint16_t text_len = strlen(text);
    uint16_t frame_len = ws_create_frame(client->tx_buffer, WS_OPCODE_TEXT,
                                       (const uint8_t*)text, text_len, true);

    int32_t sent = send(client->socket_num, client->tx_buffer, frame_len);
    return (sent == frame_len) ? 0 : -1;
}

// Send binary frame
int8_t ws_client_send_binary(websocket_client_t *client, const uint8_t *data, uint16_t len) {
    if (!client || !data || client->state != WS_STATE_CONNECTED) {
        return -1;
    }

    uint16_t frame_len = ws_create_frame(client->tx_buffer, WS_OPCODE_BINARY,
                                       data, len, true);

    int32_t sent = send(client->socket_num, client->tx_buffer, frame_len);
    return (sent == frame_len) ? 0 : -1;
}

// Create WebSocket frame
static uint16_t ws_create_frame(uint8_t *buffer, ws_opcode_t opcode, const uint8_t *payload, uint16_t payload_len, bool mask) {
    uint16_t pos = 0;

    // First byte: FIN + opcode
    buffer[pos++] = 0x80 | (opcode & 0x0F);

    // Second byte: MASK + payload length
    if (payload_len < 126) {
        buffer[pos++] = (mask ? 0x80 : 0x00) | payload_len;
    } else {
        buffer[pos++] = (mask ? 0x80 : 0x00) | 126;
        buffer[pos++] = (payload_len >> 8) & 0xFF;
        buffer[pos++] = payload_len & 0xFF;
    }

    // Masking key (if masked)
    uint8_t masking_key[4];
    if (mask) {
        for (int i = 0; i < 4; i++) {
            masking_key[i] = (uint8_t)(ws_rand() & 0xFF);
            buffer[pos++] = masking_key[i];
        }
    }

    // Payload
    if (payload && payload_len > 0) {
        if (mask) {
            for (uint16_t i = 0; i < payload_len; i++) {
                buffer[pos++] = payload[i] ^ masking_key[i % 4];
            }
        } else {
            memcpy(&buffer[pos], payload, payload_len);
            pos += payload_len;
        }
    }

    return pos;
}

// Receive WebSocket frame
int8_t ws_client_receive(websocket_client_t *client, uint8_t *buffer, uint16_t *len, ws_opcode_t *opcode) {
    if (!client || !buffer || !len || !opcode || client->state != WS_STATE_CONNECTED) {
        return -1;
    }

    // Check if data is available
    uint16_t recv_size;
    getsockopt(client->socket_num, SO_RECVBUF, &recv_size);
    if (recv_size == 0) {
        return 0; // No data available
    }

    // Receive frame header first
    int32_t received = recv(client->socket_num, client->rx_buffer, sizeof(client->rx_buffer));
    if (received <= 0) {
        return -1;
    }

    // Parse frame
    ws_frame_t frame;
    if (ws_parse_frame(client->rx_buffer, received, &frame) < 0) {
        return -1;
    }

    *opcode = frame.opcode;

    // Handle different frame types
    switch (frame.opcode) {
        case WS_OPCODE_TEXT:
        case WS_OPCODE_BINARY:
            if (frame.payload_len > *len) {
                *len = 0;
                return -1; // Buffer too small
            }
            memcpy(buffer, frame.payload, frame.payload_len);
            *len = frame.payload_len;
            break;

        case WS_OPCODE_PING:
            // Respond with pong
            ws_create_frame(client->tx_buffer, WS_OPCODE_PONG, frame.payload, frame.payload_len, true);
            send(client->socket_num, client->tx_buffer,
                 ws_create_frame(client->tx_buffer, WS_OPCODE_PONG, frame.payload, frame.payload_len, true));
            *len = 0;
            break;

        case WS_OPCODE_CLOSE:
            client->state = WS_STATE_CLOSING;
            *len = 0;
            break;

        default:
            *len = 0;
            break;
    }

    return 1;
}

// Parse WebSocket frame
static int8_t ws_parse_frame(const uint8_t *buffer, uint16_t buffer_len, ws_frame_t *frame) {
    if (buffer_len < 2) {
        return -1;
    }

    uint16_t pos = 0;

    // First byte
    frame->fin = (buffer[pos] & 0x80) != 0;
    frame->opcode = (ws_opcode_t)(buffer[pos] & 0x0F);
    pos++;

    // Second byte
    frame->mask = (buffer[pos] & 0x80) != 0;
    uint8_t payload_len = buffer[pos] & 0x7F;
    pos++;

    // Extended payload length
    if (payload_len == 126) {
        if (buffer_len < pos + 2) return -1;
        frame->payload_len = (buffer[pos] << 8) | buffer[pos + 1];
        pos += 2;
    } else if (payload_len == 127) {
        // 64-bit length not supported in this simple implementation
        return -1;
    } else {
        frame->payload_len = payload_len;
    }

    // Masking key
    if (frame->mask) {
        if (buffer_len < pos + 4) return -1;
        memcpy(frame->masking_key, &buffer[pos], 4);
        pos += 4;
    }

    // Payload
    if (buffer_len < pos + frame->payload_len) {
        return -1;
    }

    frame->payload = (uint8_t*)&buffer[pos];

    // Unmask payload if needed
    if (frame->mask) {
        ws_mask_payload(frame->payload, frame->payload_len, frame->masking_key);
    }

    return 0;
}

// Mask/unmask payload
static void ws_mask_payload(uint8_t *payload, uint16_t len, const uint8_t *mask) {
    for (uint16_t i = 0; i < len; i++) {
        payload[i] ^= mask[i % 4];
    }
}

// Send ping frame
int8_t ws_client_ping(websocket_client_t *client) {
    if (!client || client->state != WS_STATE_CONNECTED) {
        return -1;
    }

    uint16_t frame_len = ws_create_frame(client->tx_buffer, WS_OPCODE_PING, NULL, 0, true);
    int32_t sent = send(client->socket_num, client->tx_buffer, frame_len);
    return (sent == frame_len) ? 0 : -1;
}

// Close WebSocket connection
int8_t ws_client_close(websocket_client_t *client) {
    if (!client) {
        return -1;
    }

    if (client->state == WS_STATE_CONNECTED) {
        // Send close frame
        uint16_t frame_len = ws_create_frame(client->tx_buffer, WS_OPCODE_CLOSE, NULL, 0, true);
        send(client->socket_num, client->tx_buffer, frame_len);
        client->state = WS_STATE_CLOSING;
    }

    close(client->socket_num);
    client->state = WS_STATE_DISCONNECTED;
    return 0;
}

// Process WebSocket client state machine
void ws_client_process(websocket_client_t *client) {
    if (!client) return;

    switch (client->state) {
        case WS_STATE_HANDSHAKE: {
            // Check for handshake response
            uint16_t recv_size;
            getsockopt(client->socket_num, SO_RECVBUF, &recv_size);
            if (recv_size > 0) {
                int32_t received = recv(client->socket_num, client->rx_buffer, sizeof(client->rx_buffer) - 1);
                if (received > 0) {
                    client->rx_buffer[received] = '\0';
                    if (ws_parse_handshake_response(client, (char*)client->rx_buffer) == 0) {
                        client->state = WS_STATE_CONNECTED;
                    } else {
                        client->state = WS_STATE_ERROR;
                        close(client->socket_num);
                    }
                }
            }
            break;
        }

        case WS_STATE_CLOSING: {
            // Wait for close confirmation or timeout
            uint16_t recv_size;
            getsockopt(client->socket_num, SO_RECVBUF, &recv_size);
            if (recv_size > 0) {
                recv(client->socket_num, client->rx_buffer, sizeof(client->rx_buffer));
                // Could parse close frame here
            }
            // For simplicity, just close after sending close frame
            close(client->socket_num);
            client->state = WS_STATE_DISCONNECTED;
            break;
        }

        default:
            break;
    }
}
