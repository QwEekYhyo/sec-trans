#ifndef SECTRANS_COMMANDS_H
#define SECTRANS_COMMANDS_H

typedef enum {
    LIST_REQUEST = 'L',
    DOWNLOAD_REQUEST = 'D',
    UPLOAD_REQUEST = 'U',
} RequestType;

typedef enum {
    LIST_RESPONSE = 'l',
    DOWNLOAD_RESPONSE = 'd',
    FILE_CHUNK = 'f',
    UPLOAD_RESPONSE = 'u',
} ResponseType;

#endif // SECTRANS_COMMANDS_H
