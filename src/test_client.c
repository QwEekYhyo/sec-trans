#include <client.h>
#include <codes.h>
#include <common_defs.h>
#include <message.h>
#include <server.h>

#include <stdint.h>
#include <stdio.h>

static char msg[1024];

int main() {
    if (startserver(RESPONSE_PORT) != 0)
        printf("Failed to start the client.\n");

    set_message_code(msg, LIST_REQUEST);
    write_size_to_message(msg, 0);
    printf("Sending message to server...\n");
    int error_code = sndmsg(msg, SERVER_PORT);

    printf("Sent message with code: %d\n", error_code);
    printf("Waiting for response...\n");

    char buffer[1024] = {0};
    getmsg(buffer);
    printf("Got response!\n");
    printf("Response type: %c\n", get_message_code(buffer));

    uint32_t size = read_size_from_message(buffer);
    
    printf("Response size: %u\n", size);

    printf("Response body:\n");
    for (uint32_t i = 0; i < size; i++)
        putchar(buffer[i + HEADER_SIZE]);
    putchar('\n');

    stopserver();

    return 0;
}
