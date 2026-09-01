#ifndef LIBC_MEMORY_H
#define LIBC_MEMORY_H

#define size_t unsigned long

void *malloc(size_t size);
void free(void *ptr);

#endif
