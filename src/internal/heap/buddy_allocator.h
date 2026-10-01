#ifndef LIBC_BUDDY_ALLOCATOR_H
#define LIBC_BUDDY_ALLOCATOR_H

#include <stddef.h>

void init_buddy_allocator(uintptr_t heap_start);
uint64_t page_alloc(size_t n_pages);
void page_free(uint64_t ptr, size_t n_pages);

#endif
