#ifndef LIBC_HEAP_H
#define LIBC_HEAP_H

#include <stdint.h>
void libc_heap_init();
uint64_t log2_rounded_up(uint64_t num);

#define HEAP_SIZE_ORDER 3

#endif
