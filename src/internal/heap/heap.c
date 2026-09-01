#include "heap.h"
#include "syscalls/memory.h"
#include "syscalls/proc.h"
#include "syscalls/syscall_generic.h"
#include <stdint.h>
#include <stddef.h>

static void *heap_start = NULL;
static uint64_t heap_region_id = 0;

void libc_heap_init() {
    uint64_t addr = 0;
    uint8_t order = 3; //max 512GB heap
    uint8_t permissions = 0b01; //no execute, write
    uint8_t region_type = 2; //heap
    uint64_t management_mode = 0; //managed by kernel, growing up
    uint64_t region_name_len = 4;
    uint8_t *region_name = (uint8_t *) "heap";
    syscall_2ret ret = _make_region(addr, order, permissions, region_type, management_mode, region_name_len, region_name);

    if (ret.ret0 == -1) {
        _exit(1);
    }

    heap_region_id = ret.ret0;
    heap_start = (void *) ret.ret1;
}

void *malloc(size_t size) {
    void *start = heap_start;
    //align start to 8B
    if ((uintptr_t)start % 8 != 0) {
        start = (void *)(((uintptr_t)start + 7) & ~((uintptr_t)7));
    }
    heap_start = (void *)((uintptr_t)start + size);
    return start;
}

void free(void *ptr) {
    //do nothing
}
