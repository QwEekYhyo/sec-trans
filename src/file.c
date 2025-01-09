#include <client.h>
#include <codes.h>
#include <common_defs.h>
#include <message.h>
#include <file.h>
#include <server.h>

#include <fcntl.h>
#include <libgen.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void send_file(char buffer[PACKET_SIZE], const char* file_name, unsigned int port) {
    char sanitized_file_name[MAX_FILENAME_LENGTH + 6] = "dist/";
    const char* base_name = basename((char*) file_name);
    strncat(sanitized_file_name, base_name, MAX_FILENAME_LENGTH);

    int fd = open(sanitized_file_name, O_RDONLY);
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

void receive_file(char buffer[PACKET_SIZE], const char* file_name) {
    char sanitized_file_name[MAX_FILENAME_LENGTH + 6] = "dist/";
    const char* base_name = basename((char*) file_name);
    strncat(sanitized_file_name, base_name, MAX_FILENAME_LENGTH);

    int fd = open(sanitized_file_name, O_WRONLY | O_CREAT | O_EXCL | O_APPEND, 0644);
    if (fd == -1) {
        send_error(buffer, "Server could not create file", RESPONSE_PORT);
        return;
    }

    ResponseType code;
    uint32_t chunk_size;
    /* Need to add a timeout here */
    do {
        getmsg(buffer);
        code = get_message_code(buffer);
        if (code == FILE_CHUNK) {
            chunk_size = read_size_from_message(buffer);
            write(fd, buffer + HEADER_SIZE, chunk_size);
        }
    } while (code != UPLOAD_DONE && code != ERROR);

    close(fd);

    if (code == UPLOAD_DONE) {
        set_message_code(buffer, UPLOAD_RESPONSE);
        buffer[HEADER_SIZE] = 'O';
        buffer[HEADER_SIZE + 1] = 'K';
        write_size_to_message(buffer, 2);
        sndmsg(buffer, RESPONSE_PORT);
    } else // ERROR
        send_error(buffer, "Error during upload", RESPONSE_PORT);
}
