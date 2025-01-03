#include "../include/server.h"

#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Starting server on port 8080...\n");
    if (startserver(8080) == 0) {
        printf("Server started successfully.\n");
    } else {
        printf("Failed to start the server.\n");
    }

    char buffer[1024];
    int error_code;
    while (1) {
        error_code = getmsg(buffer);
        printf("Received message with status: %d\n", error_code);
        printf("Message is:\n");
        printf("%s\n", buffer);
        sleep(1);
    }

    printf("Stopping server...\n");
    if (stopserver() == 0) {
        printf("Server stopped successfully.\n");
    } else {
        printf("Failed to stop the server.\n");
    }

    return 0;
}
