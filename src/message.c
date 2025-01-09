#include <client.h>
#include <codes.h>
#include <message.h>

#include <string.h>
#include <stdio.h>

void write_size_to_message(char* msg, uint32_t size) {
    const char hex_chars[] = "0123456789ABCDEF";

    msg += HEADER_CODE_SIZE;
    for (int i = HEADER_LENGTH_SIZE - 1; i >= 0; i--) {
        msg[i] = hex_chars[size & 0xF];
        size >>= 4;
    }
}

uint32_t read_size_from_message(const char* msg) {
    char hex_size[HEADER_LENGTH_SIZE + 1];
    memcpy(hex_size, msg + HEADER_CODE_SIZE, HEADER_LENGTH_SIZE);
    uint32_t size;
    sscanf(hex_size, "%X", &size);
    return size;
}

char get_message_code(const char* message) {
    return *message;
}

void set_message_code(char* message, char code) {
    message[0] = code;
}

int send_error(char buffer[PACKET_SIZE], const char* error_message, unsigned int port) {
    set_message_code(buffer, ERROR);
    const unsigned int len = strlen(error_message);
    memcpy(buffer + HEADER_SIZE, error_message, len);
    write_size_to_message(buffer, len);
    return sndmsg(buffer, port);
}
