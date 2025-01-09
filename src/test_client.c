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

int main() {
    if (startserver(RESPONSE_PORT) != 0)
        printf("Failed to start the client.\n");

    /* List test */

    set_message_code(msg, LIST_REQUEST);
    write_size_to_message(msg, 0);
    printf("Sending message to server...\n");
    int error_code = sndmsg(msg, SERVER_PORT);

    printf("Sent message with code: %d\n", error_code);
    printf("Waiting for response...\n");

    char buffer[PACKET_SIZE] = {0};
    getmsg(buffer);
    debug_response(buffer);

    /* Download test */

    set_message_code(msg, DOWNLOAD_REQUEST);
    char* file = "../secretfolder/../../thing";
    unsigned int len = strlen(file);
    write_size_to_message(msg, len);
    for (int i = 0; i < len; i++)
        msg[i + HEADER_SIZE] = file[i];
    msg[HEADER_SIZE + len] = '\0';
    sndmsg(msg, SERVER_PORT);
    printf("Waiting for response...\n");

    ResponseType code;
    do {
        getmsg(buffer);
        code = get_message_code(buffer);
        debug_response(buffer);
    } while (code != DOWNLOAD_RESPONSE && code != ERROR);

    /* Upload test */
    set_message_code(msg, UPLOAD_REQUEST);
    file = "testfile.txt";
    len = strlen(file);
    write_size_to_message(msg, len);
    for (int i = 0; i < len; i++)
        msg[i + HEADER_SIZE] = file[i];

    msg[HEADER_SIZE + len] = '\0';
    sndmsg(msg, SERVER_PORT);
    printf("Waiting for response...\n");

    int fd = open(file, O_RDONLY);
    ssize_t read_bytes;
    while (1) {
        read_bytes = read(fd, msg + HEADER_SIZE, MAX_BODY_SIZE);
        if (read_bytes == -1 || read_bytes == 0) break;

        set_message_code(msg, FILE_CHUNK);
        write_size_to_message(msg, read_bytes);
        sndmsg(msg, SERVER_PORT);
        if (read_bytes < MAX_BODY_SIZE) break;
    }

    close(fd);

    set_message_code(msg, UPLOAD_DONE);
    write_size_to_message(msg, 0);
    sndmsg(msg, SERVER_PORT);

    getmsg(buffer);
    debug_response(buffer);

    stopserver();

    return 0;
}
