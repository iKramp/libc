#include "stdio.h"
#include "stdio/stdio_common.h"
#include <stdlib.h>

void setbuf(FILE *stream, char *buf) {
    int mode = (buf == NULL) ? _IONBF : _IOFBF;
    setvbuf(stream, buf, mode, BUFSIZ);
}

int setvbuf(FILE *stream, char *buf, int mode, size_t size) {
    if (stream == NULL) {
        return -1;
    }

    switch (mode) {
        case _IONBF:
            stream->buffer_mode = BUFFERED_NONE;
            break;
        case _IOLBF:
            stream->buffer_mode = BUFFERED_LINE;
            break;
        case _IOFBF:
            stream->buffer_mode = BUFFERED_FULL;
            break;
        default:
            return -1;
    }

    if (stream->buffer != NULL) {
        free(stream->buffer);
        stream->buffer = NULL;
    }

    uint8_t *buffer_to_use = NULL;
    if (buf != NULL) {
        buffer_to_use = (uint8_t *)buf;
    } else {
        buffer_to_use = malloc(size);
        if (buffer_to_use == NULL) {
            return -1;
        }
    }

    stream->buffer = buffer_to_use;
    stream->buffer_size = size;
    return 0;
}
