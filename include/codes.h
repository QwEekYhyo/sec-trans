#ifndef SECTRANS_CODES_H
#define SECTRANS_CODES_H

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

#endif // SECTRANS_CODES_H
