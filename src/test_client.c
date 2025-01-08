#include <client.h>
#include <commands.h>
#include <common_defs.h>

#include <stdio.h>

static char msg[1024];

int main() {
    msg[0] = LIST_REQUEST;
    msg[1] = msg[2] = 0;
    msg[3] = 4;
    printf("Sending message to server...\n");
    int error_code = sndmsg(msg, SERVER_PORT);

    printf("Sent message with code: %d\n", error_code);

    return 0;
}
