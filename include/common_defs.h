#ifndef SECTRANS_COMMON_DEFS_H
#define SECTRANS_COMMON_DEFS_H

#define SERVER_PORT 8080
// RESPONSE_PORT is temporary, they should change at every request
#define RESPONSE_PORT 8888

#define HEADER_CODE_SIZE 1
#define HEADER_LENGTH_SIZE 6
#define HEADER_SIZE HEADER_CODE_SIZE + HEADER_LENGTH_SIZE

#define PACKET_SIZE 1024
#define MAX_BODY_SIZE PACKET_SIZE - HEADER_SIZE

#endif // SECTRANS_COMMON_DEFS_H
