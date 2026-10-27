#ifndef LIBC_INTERNAL_FILEIO_WRITE_H
#define LIBC_INTERNAL_FILEIO_WRITE_H

#include "stdio.h"

struct ObjWriteResult {
    size_t whole_elements;
    size_t partial_bytes_written;
};

//force_flush: true ignores buffering by newline
int drain_buffer(FILE *stream, uint8_t force_flush);
//doesn't perform OS writes, doesn't perform partial writes
struct ObjWriteResult write_buffer(FILE *stream, void *ptr, size_t size, size_t nmemb);


#endif

