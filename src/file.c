#include <client.h>
#include <codes.h>
#include <common_defs.h>
#include <message.h>
#include <file.h>

#include <fcntl.h>

void send_file(char buffer[PACKET_SIZE], const char* file_name, unsigned int port) {
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        send_error(buffer, "File does not exists", port);
        return;
    }


}
