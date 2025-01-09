#include <client.h>
#include <codes.h>
#include <common_defs.h>
#include <message.h>
#include <server.h>

#include <stdint.h>
#include <stdio.h>

static char msg[1024];

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

    char buffer[1024] = {0};
    getmsg(buffer);
    debug_response(buffer);

    /* Download test */

    set_message_code(msg, DOWNLOAD_REQUEST);
    const char* file = "dist/thing";
    write_size_to_message(msg, 10);
    for (int i = 0; i < 10; i++)
        msg[i + HEADER_SIZE] = file[i];
    msg[HEADER_SIZE + 10] = '\0';
    sndmsg(msg, SERVER_PORT);
    printf("Waiting for response...\n");

    getmsg(buffer);
    debug_response(buffer);

    stopserver();

    return 0;
}
