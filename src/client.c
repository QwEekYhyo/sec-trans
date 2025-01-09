#include <client.h>
#include <codes.h>
#include <common_defs.h>
#include <message.h>
#include <server.h>

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static char msg[PACKET_SIZE];

void debug_response(const char* buffer) {
    printf("Got response!\n");
    printf("Response type: %c\n", get_message_code(buffer));

    uint32_t size = read_size_from_message(buffer);
    
    printf("Response size: %u\n", size);

    printf("Response body:\n");
    for (uint32_t i = 0; i < size; i++)
        putchar(buffer[i + HEADER_SIZE]);
    putchar('\n');
}

void print_usage(const char* executable_name) {
    printf("Usage:\n");
    printf("  %s -list\n", executable_name);
    printf("  %s -down <filename>\n", executable_name);
    printf("  %s -up <filename>\n", executable_name);
}

void list_files() {
    set_message_code(msg, LIST_REQUEST);
    write_size_to_message(msg, 0);

    printf("Sending list request to server...\n");
    sndmsg(msg, SERVER_PORT);

    char buffer[PACKET_SIZE] = {0};
    getmsg(buffer);
    debug_response(buffer);
}

void download_file(const char* file) {
    set_message_code(msg, DOWNLOAD_REQUEST);
    unsigned int len = strlen(file);
    write_size_to_message(msg, len);
    strncpy(msg + HEADER_SIZE, file, len);
    msg[HEADER_SIZE + len] = '\0';

    printf("Sending download request for file: %s\n", file);
    sndmsg(msg, SERVER_PORT);

    int fd = open(file, O_WRONLY | O_CREAT | O_EXCL | O_APPEND, 0644);
    if (fd == -1) {
        perror("Failed to create file");
        return;
    }

    char buffer[PACKET_SIZE] = {0};
    ResponseType code;
    uint32_t chunk_size;

    printf("Waiting for file chunks...\n");
    do {
        getmsg(buffer);
        code = get_message_code(buffer);
        if (code == FILE_CHUNK) {
            chunk_size = read_size_from_message(buffer);
            if (write(fd, buffer + HEADER_SIZE, chunk_size) == -1) {
                perror("Failed to write to file");
                close(fd);
                return;
            }
        }
    } while (code != DOWNLOAD_RESPONSE && code != ERROR);

    close(fd);

    if (code == DOWNLOAD_RESPONSE)
        printf("File downloaded successfully: %s\n", file);
    else
        printf("Error occurred during download\n");
}

void upload_file(const char* file) {
    set_message_code(msg, UPLOAD_REQUEST);
    unsigned int len = strlen(file);
    write_size_to_message(msg, len);
    strncpy(msg + HEADER_SIZE, file, len);
    msg[HEADER_SIZE + len] = '\0';

    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file for upload");
        return;
    }

    printf("Sending upload request for file: %s\n", file);
    sndmsg(msg, SERVER_PORT);

    ssize_t read_bytes;
    while ((read_bytes = read(fd, msg + HEADER_SIZE, MAX_BODY_SIZE)) > 0) {
        set_message_code(msg, FILE_CHUNK);
        write_size_to_message(msg, read_bytes);
        sndmsg(msg, SERVER_PORT);
    }
    close(fd);

    set_message_code(msg, UPLOAD_DONE);
    write_size_to_message(msg, 0);
    sndmsg(msg, SERVER_PORT);

    char buffer[PACKET_SIZE] = {0};
    getmsg(buffer);
    debug_response(buffer);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    if (startserver(RESPONSE_PORT) != 0) {
        printf("Failed to start the client.\n");
        return 1;
    }

    if (strcmp(argv[1], "-list") == 0) {
        list_files();
    } else if (strcmp(argv[1], "-down") == 0 && argc >= 3) {
        download_file(argv[2]);
    } else if (strcmp(argv[1], "-up") == 0 && argc >= 3) {
        upload_file(argv[2]);
    } else {
        printf("Invalid arguments.\n");
        print_usage(argv[0]);
        stopserver();
        return 1;
    }

    stopserver();
    return 0;
}
