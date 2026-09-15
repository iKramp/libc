#include "syscalls/filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_stream_members(FILE *stream) {
    if (stream->buffer != NULL) {
        free(stream->buffer);
        stream->buffer = NULL;
    }
    if (stream->filename != NULL) {
        free(stream->filename);
        stream->filename = NULL;
    }
}

int fopen_file(FILE *stream, const char *filename, const char *mode) {
    free_stream_members(stream);
    stream->buffer_pos = 0;
    stream->reached_eof = 0;
    stream->error_occured = 0;

    uint8_t mode_num = 0;
    uint8_t binary = 0;
    uint8_t update = 0;
    for (const char *p = mode; *p != '\0'; p++) {
        switch (*p) {
            case 'r':
                mode_num = 0;
                break;
            case 'w':
                mode_num = 1;
                break;
            case 'a':
                mode_num = 2;
                break;
            case '+':
                update = 1;
                break;
            case 'b':
                binary = 1;
                break;
            default:
                free_stream_members(stream);
                free(stream);
                return -1;
        }
    }
    uint8_t read = (mode_num == 0) || update;
    uint8_t write = (mode_num == 1) || (mode_num == 2) || update;
    uint8_t append = (mode_num == 2);
    uint8_t truncate = (mode_num == 1);

    uint64_t flags = make_open_flags(read, write, append, truncate);

    int fd = _fopen(strlen(filename), filename, 0, flags);
    if (fd < 0) {
        free_stream_members(stream);
        free(stream);
        return -1;
    }

    stream->fd = fd;
    stream->buffer_size = BUFSIZ;
    stream->buffer = malloc(stream->buffer_size);
    if (stream->buffer == NULL) {
        fclose(stream);
        return -1;
    }
    stream->buffer_pos = 0;
    stream->buffer_end = 0;
    stream->blocking = 1;

    stream->filename = malloc(strlen(filename) + 1);
    if (stream->filename == NULL) {
        fclose(stream);
        return -1;
    }
    strcpy(stream->filename, filename);

    stream->file_pos = 0;

    stream->reached_eof = 0;
    stream->error_occured = 0;
    stream->open = 1;

    stream->newline_buffered = 0;

    return 0;
}

FILE *fopen(const char *filename, const char *mode) {
    FILE *stream = malloc(sizeof(FILE));
    if (stream == NULL) {
        return NULL;
    }

    int res = fopen_file(stream, filename, mode);

    if (res != 0) {
        //already freed
        return NULL;
    }

    return stream;
}

//only OS level interaaction
int fclose_without_free(FILE *stream) {
    fflush(stream);
    return _fclose(stream->fd);
}

int fclose(FILE *stream) {
    int res = fclose_without_free(stream);
    free_stream_members(stream);
    free(stream);
    return res;
}

FILE *freopen(const char *filename, const char *mode, FILE *stream) {
    const char *f_name_to_use;
    if (filename == NULL) {
        // f_name_to_use = stream->filename;
        int len = strlen(stream->filename);
        f_name_to_use = malloc(len + 1);
        if (f_name_to_use == NULL) {
            return NULL;
        }
        memcpy((void *)f_name_to_use, stream->filename, len + 1);
    } else {
        f_name_to_use = filename;
    }
    fclose_without_free(stream);

    int res = fopen_file(stream, filename, mode);

    if (res != 0) {
        //already freed
        return NULL;
    }

    return stream;
}
