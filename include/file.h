#ifndef SECTRANS_FILE_H
#define SECTRANS_FILE_H

#include <common_defs.h>

void send_file(char buffer[PACKET_SIZE], const char* file_name, unsigned int port);
void receive_file(char buffer[PACKET_SIZE], const char* file_name);

#endif // SECTRANS_FILE_H
