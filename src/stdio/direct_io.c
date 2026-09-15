#include "stdio.h"
#include "internal/file_io/read.h"
#include "internal/file_io/write.h"

size_t fread(void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream) {
    if (size == 0 || nmemb == 0) {
        return 0;
    }

    uint8_t temp_eof = false;
    size_t total_read_count = 0;

    while (1) {
        size_t read_count = read_buffer(stream, ptr, size, nmemb);
        total_read_count += read_count;
        if (read_count == nmemb || temp_eof) {
            return total_read_count;
        }
        ptr = (char *)ptr + read_count * size;
        nmemb -= read_count;

        if (stream->reached_eof) {
            //last element is partial
            size_t individual_read_cnt = read_buffer(stream, ptr, size, 1);
            if (individual_read_cnt == size) {
                //should be impossible but just in case
                total_read_count += 1;
            }
            return total_read_count;
        }

        temp_eof = fill_buffer(stream);
    }
}

size_t fwrite(const void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream) {
    if (size == 0 || nmemb == 0) {
        return 0;
    }

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
