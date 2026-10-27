#include "stdio.h"
#include "stdio/read.h"
#include "stdio/write.h"
#include "syscalls/filesystem.h"

size_t fread(void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream) {
    if (stream->buffer_direction != STREAM_DIR_READ) {
        stream->buffer_direction = STREAM_DIR_READ;
        stream->buffer_pos = 0;
        stream->buffer_end = 0;
    }

    if (size == 0 || nmemb == 0) {
        return 0;
    }

    size_t left_in_object = 0;
    while (1) {
        int size_, nmemb_;
        if (left_in_object > 0) {
            size_ = left_in_object;
            nmemb_ = 1;
        } else {
            size_ = size;
            nmemb_ = nmemb;
        }

        void *ptr_;
        if (left_in_object > 0) {
            ptr_ = (char *)ptr + (size - left_in_object);
        } else {
            ptr_ = ptr;
        }

        struct ObjReadResult result = read_buffer(stream, ptr_, size_, nmemb_);
        if (result.whole_elements > 0) {
            return result.whole_elements;
        }

        if (result.partial_bytes_read > 0) {
            if (left_in_object == 0) {
                left_in_object = size - result.partial_bytes_read;
            } else {
                left_in_object -= result.partial_bytes_read;
            }
        }

        uint8_t temp_eof = fill_buffer(stream);
        if (temp_eof && stream->reached_eof) {
            return 0; // EOF reached, no more data to read
        }
    }
}

size_t fwrite(const void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream) {
    if (stream->buffer_direction != STREAM_DIR_READ) {
        stream->buffer_direction = STREAM_DIR_WRITE;
        stream->buffer_pos = 0;
        stream->buffer_end = 0;
    }

    if (size == 0 || nmemb == 0) {
        return 0;
    }

    size_t left_to_write = nmemb;

    size_t total_written_count = 0;
    size_t left_in_object = 0;
    while (1) {
        int size_, nmemb_;
        if (left_in_object > 0) {
            size_ = left_in_object;
            nmemb_ = 1;
        } else {
            size_ = size;
            nmemb_ = left_to_write;
        }

        void *ptr_;
        if (left_in_object > 0) {
            ptr_ = (char *)ptr + (size - left_in_object);
        } else {
            ptr_ = (void *)ptr;
        }

        struct ObjWriteResult result = write_buffer(stream, (void *)ptr_, size_, nmemb_);
        total_written_count += result.whole_elements;
        left_to_write -= result.whole_elements;
        ptr = (const char *)ptr + result.whole_elements * size;
        if (total_written_count == nmemb) {
            break;
        }
        //could not write all elements
        if (result.partial_bytes_written > 0) {
            if (left_in_object == 0) {
                left_in_object = size - result.partial_bytes_written;
            } else {
                left_in_object -= result.partial_bytes_written;
            }
        }

        //force a drain, buffer has no more space
        if (drain_buffer(stream, true) != 0) {
            break;
        }
    }

    //no forcing, drain on newline
    drain_buffer(stream, false);

    return total_written_count;
}
