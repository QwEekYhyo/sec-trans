#include <commands.h>
#include <common_defs.h>
#include <server.h>

#include <stdio.h>

void handle_list_request(char* buffer) {
    printf("Someone asked to list files\n");
}

int main() {
    printf("Starting server on port 8080...\n");
    if (startserver(SERVER_PORT) == 0)
        printf("Server started successfully.\n");
    else
        printf("Failed to start the server.\n");

    char buffer[1024];
    while (1) {
        getmsg(buffer);
        if (buffer[0] == LIST_REQUEST)
            handle_list_request(buffer);
    }

    return 0;
}
