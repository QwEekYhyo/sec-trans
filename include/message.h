#ifndef SECTRANS_MESSAGE_H
#define SECTRANS_MESSAGE_H

#include <common_defs.h>

#include <stdint.h>

/* Write specified size in message buffer at the correct index in hex characters */
void write_size_to_message(char* message, uint32_t size);
/* Read size from hex characters in message buffer */
uint32_t read_size_from_message(const char* message);

char get_message_code(const char* message);
void set_message_code(char* message, char code);

int send_error(char buffer[PACKET_SIZE], const char* error_message, unsigned int port);

#endif // SECTRANS_MESSAGE_H
