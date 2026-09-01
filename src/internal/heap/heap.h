#ifndef LIBC_HEAP_H
#define LIBC_HEAP_H

#include <stddef.h>

void libc_heap_init();
void *malloc(size_t size);
void free(void *ptr);

#endif
