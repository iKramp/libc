#include "stdio.h"
#include "stdio/read.h"
#include "stdio/write.h"
#include "syscalls/filesystem.h"

size_t fread(void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream) {
    if (size == 0 || nmemb == 0) {
        return 0;
    }

    while (1) {
        size_t read_count = read_buffer(stream, ptr, size, nmemb);
        if (read_count > 0) {
            return read_count;
        }

        uint8_t temp_eof = fill_buffer(stream);
        if (temp_eof && stream->reached_eof) {
            return 0; // EOF reached, no more data to read
        }
    }
}

size_t fwrite(const void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream) {
    if (size == 0 || nmemb == 0) {
        return 0;
    }

    //direct write for testing
    return _fwrite(stream->fd, size * nmemb, ptr);
    size_t total_written_count = 0;
    while (1) {
        size_t written_count = write_buffer(stream, (void *)ptr, size, nmemb);
        total_written_count += written_count;
        if (written_count == nmemb) {
            break;
        }
        ptr = (const char *)ptr + written_count * size;
        nmemb -= written_count;

        if (drain_buffer(stream, false) != 0) {
            break;
        }
    }

    drain_buffer(stream, false);

    return total_written_count;
}
