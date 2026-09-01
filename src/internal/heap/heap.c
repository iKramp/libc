#include "heap.h"
#include "syscalls/memory.h"
#include <stdint.h>

static void *heap_start = NULL;

void libc_heap_init() {
    uint64_t addr = 0;
    uint8_t order = 3; //max 512GB heap
    uint8_t permissions = 0b01; //no execute, write
    uint8_t region_type = 2; //heap
    uint64_t management_mode = 0; //managed by kernel, growing up
    uint64_t region_name_len = 4;
    uint8_t *region_name = (uint8_t *) "heap";
    _make_region(addr, order, permissions, region_type, management_mode, region_name_len, region_name);
}
