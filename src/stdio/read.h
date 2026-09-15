#ifndef LIBC_INTERNAL_FILEIO_READ_H
#define LIBC_INTERNAL_FILEIO_READ_H

#include "stdio.h"

//true if temporary EOF is reached. Always false if file is opened in blocking mode
uint8_t fill_buffer(FILE *stream);
//doesn't perform OS reads, doesn't perform partial reads
size_t read_buffer(FILE *stream, void *ptr, size_t size, size_t nmemb);


#endif
