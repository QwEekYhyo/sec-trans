#include "../include/client.h"

#include <dlfcn.h>
#include <stdio.h>

static char* msg = "Hello, world!";

int main() {
    printf("Sending message to server...\n");
    int error_code = sndmsg(msg, 8080);

    printf("Sent message with code: %d\n", error_code);

    return 0;
}
