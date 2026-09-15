#include "write.h"
#include "syscalls/syscall_generic.h"
#include "syscalls/filesystem.h"
#include <string.h>

static void move_data_to_start(FILE *stream) {
    if (stream->buffer_pos > 0) {
        memmove(stream->buffer, stream->buffer + stream->buffer_pos, stream->buffer_end - stream->buffer_pos);
        stream->buffer_end -= stream->buffer_pos;
        stream->buffer_pos = 0;
    }
}

//force_flush: true ignores buffering by newline
int drain_buffer(FILE *stream, uint8_t force_flush) {
    force_flush |= !stream->newline_buffered;
    size_t cnt;
    if (force_flush) {
        cnt = stream->buffer_end - stream->buffer_pos;
    } else {
        void *pos = memchr(stream->buffer + stream->buffer_pos, '\n', stream->buffer_end - stream->buffer_pos);
        if (pos == NULL) {
            cnt = stream->buffer_end - stream->buffer_pos;
        } else {
            cnt = pos - (void *)(stream->buffer + stream->buffer_pos) + 1; //include newline
        }
    }

    return _fwrite(stream->fd, cnt, stream->buffer);
    
}

//doesn't perform OS writes, doesn't perform partial writes
size_t write_buffer(FILE *stream, void *ptr, size_t size, size_t nmemb) {
    move_data_to_start(stream);
    size_t available_space = stream->buffer_size - stream->buffer_end;
    size_t available_elements = available_space / size;
    size_t n_elements = available_elements < nmemb ? available_elements : nmemb;
    memcpy(stream->buffer + stream->buffer_end, ptr, n_elements * size);
    stream->buffer_end += n_elements * nmemb;
    return n_elements;
}

