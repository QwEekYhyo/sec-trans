#include <common_defs.h>
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

uint32_t read_size_from_message(char* msg) {
    char hex_size[HEADER_LENGTH_SIZE + 1];
    memcpy(hex_size, msg + HEADER_CODE_SIZE, HEADER_LENGTH_SIZE);
    uint32_t size;
    sscanf(hex_size, "%X", &size);
    return size;
}

char get_message_code(char* message) {
    return *message;
}

void set_message_code(char* message, char code) {
    message[0] = code;
}
