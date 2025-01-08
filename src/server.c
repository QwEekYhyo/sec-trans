#include <client.h>
#include <codes.h>
#include <common_defs.h>
#include <message.h>
#include <server.h>

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void handle_list_request(char* buffer) {
    printf("Someone asked to list files\n");

    DIR* dir = opendir("dist");
    if (!dir) {
        printf("Failed to open directory 'dist'.\n");
        return;
    }

    set_message_code(buffer, LIST_RESPONSE);

    unsigned int buffer_pos = 7;
    uint32_t total_size = 0;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        unsigned int file_name_len = strlen(entry->d_name);
        // If the filename would take too much space, discard it
        // TODO: do this better
        if (buffer_pos + file_name_len + 1 > 1024)
            continue;

        strncpy(buffer + buffer_pos, entry->d_name, file_name_len);
        buffer_pos += file_name_len;
        buffer[buffer_pos++] = '\n';
        total_size += file_name_len + 1;
    }

    closedir(dir);

    write_size_to_message(buffer, total_size);

    sndmsg(buffer, RESPONSE_PORT);
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
        if (get_message_code(buffer) == LIST_REQUEST)
            handle_list_request(buffer);
    }

    return 0;
}
