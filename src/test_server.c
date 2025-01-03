#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    void* handle;
    int (*startserver)(int);
    int (*stopserver)();
    int (*getmsg)(char*);
    char* error;

    handle = dlopen("./libserver.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    *(void**)(&startserver) = dlsym(handle, "startserver");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

    *(void**)(&stopserver) = dlsym(handle, "stopserver");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

    *(void**)(&getmsg) = dlsym(handle, "getmsg");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

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

    dlclose(handle);
    return 0;
}
