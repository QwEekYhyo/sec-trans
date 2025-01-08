#ifndef SECTRANS_MESSAGE_H
#define SECTRANS_MESSAGE_H

#include <stdint.h>

/* Write specified size in message buffer at the correct index in hex characters */
void write_size_to_message(char* message, uint32_t size);
/* Read size from hex characters in message buffer */
uint32_t read_size_from_message(char* message);

#endif // SECTRANS_MESSAGE_H
