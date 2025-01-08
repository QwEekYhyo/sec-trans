#include <message.h>

#include <string.h>
#include <stdio.h>

void write_size_to_message(char* msg, uint32_t size) {
    const char hex_chars[] = "0123456789ABCDEF";

    msg++;
    for (int i = 5; i >= 0; i--) {
        msg[i] = hex_chars[size & 0xF];
        size >>= 4;
    }
}

uint32_t read_size_from_message(char* msg) {
    char hex_size[7];
    memcpy(hex_size, msg + 1, 6);
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
