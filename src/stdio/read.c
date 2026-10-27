#include "read.h"
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

//true if temporary EOF is reached. Always false if file is opened in blocking mode
uint8_t fill_buffer(FILE *stream) {
    move_data_to_start(stream);

    syscall_2ret ret = _fread(stream->fd, stream->buffer_size - stream->buffer_end, stream->buffer + stream->buffer_end, make_read_flags(stream->blocking));
    uint64_t bytes_read = ret.ret0;
    uint64_t read_result = ret.ret1;

    if (read_result == READ_RESULT_PERMANENT_EOF) {
        stream->reached_eof = 1;
    }

    stream->buffer_end += bytes_read;

    return (read_result == READ_RESULT_TEMPORARY_EOF) && !stream->blocking;
}

//doesn't perform OS reads, doesn't perform partial reads
struct ObjReadResult read_buffer(FILE *stream, void *ptr, size_t size, size_t nmemb) {
    if (size > stream->buffer_size) {
        //perform a partial read
        memcpy(ptr, stream->buffer + stream->buffer_pos, stream->buffer_end - stream->buffer_pos);
        struct ObjReadResult result = {0, stream->buffer_end - stream->buffer_pos};
        stream->buffer_pos = stream->buffer_end;
        return result;
    }

    size_t requested_size = size * nmemb;
    size_t stream_size = stream->buffer_end - stream->buffer_pos;
    size_t max_size = stream_size < requested_size ? stream_size : requested_size;
    size_t n_elements = max_size / size;
    memcpy(ptr, stream->buffer + stream->buffer_pos, size * n_elements);
    stream->buffer_pos += n_elements * size;
    return (struct ObjReadResult){n_elements, 0};
}
