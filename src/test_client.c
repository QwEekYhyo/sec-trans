#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

static char* msg = "Hello, world!";

int main() {
    void* handle;
    int (*sndmsg)(char*, int port);
    char* error;

    handle = dlopen("./libclient.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    *(void**)(&sndmsg) = dlsym(handle, "sndmsg");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

    printf("Sending message to server...\n");
    int error_code = sndmsg(msg, 8080);

    printf("Sent message with code: %d\n", error_code);

    dlclose(handle);
    return 0;
}
