#include <client.h>
#include <codes.h>
#include <common_defs.h>
#include <message.h>
#include <file.h>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void send_file(char buffer[PACKET_SIZE], const char* file_name, unsigned int port) {
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        send_error(buffer, "File does not exists", port);
        return;
    }

    ssize_t read_bytes;
    while (1) {
        read_bytes = read(fd, buffer + HEADER_SIZE, MAX_BODY_SIZE);
        if (read_bytes == -1 || read_bytes == 0) break;

        set_message_code(buffer, FILE_CHUNK);
        write_size_to_message(buffer, read_bytes);
        sndmsg(buffer, port);
        if (read_bytes < MAX_BODY_SIZE) break;
    }

    close(fd);

    set_message_code(buffer, DOWNLOAD_RESPONSE);
    buffer[HEADER_SIZE] = 'O';
    buffer[HEADER_SIZE + 1] = 'K';
    write_size_to_message(buffer, 2);
    sndmsg(buffer, port);
}
