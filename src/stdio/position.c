#include "stdio.h"
#include "syscalls/filesystem.h"

int fseek(FILE *stream, long offset, int whence) {
    fflush(stream); // Flush the stream before seeking
    int file_seek_mode;
    if (whence == SEEK_SET) {
        file_seek_mode = 0;
    } else if (whence == SEEK_CUR) {
        file_seek_mode = 1;
    } else if (whence == SEEK_END) {
        file_seek_mode = 2;
    } else {
        return -1; // Invalid whence value
    }


    stream->file_pos = _fseek(stream->fd, offset, file_seek_mode);
    return 0;
}

long int ftell(FILE *stream) {
    return stream->file_pos;
}
