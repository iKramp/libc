#ifndef LIBC_SYSCALL_MEMORY_H
#define LIBC_SYSCALL_MEMORY_H

#include "stdint.h"

static inline uint8_t make_mem_range_permissions(uint8_t write, uint8_t execute) {
    return ((write != 0) << 0) | ((execute != 0) << 1);
}
static inline uint8_t is_mem_range_write(uint8_t permissions) {
    return (permissions & 1) != 0;
}
static inline uint8_t is_mem_range_execute(uint8_t permissions) {
    return (permissions & 2) != 0;
}

typedef enum {
    STACK = 0,
    CODE = 1,
    DATA = 2,
    SHARED = 3,
} MemoryRegionType;

typedef enum {
    KERNEL = 0,
    USER = 1,
} MemoryManagementMode;
typedef enum {
    GROW_UP = 0,
    GROW_DOWN = 1,
} KernelManagedMemorySubmode;

static inline uint64_t make_management_mode(uint8_t mode, uint8_t submode) {
    return ((uint64_t)mode << 0) | ((uint64_t)submode << 32);
}

int _make_region(uint64_t address, uint8_t order, uint8_t permissions,
                 uint8_t region_type, uint64_t management_mode,
                 uint64_t name_len, uint8_t *name_ptr);
int _remove_region(uint64_t region_id);
int _set_protection(uint64_t region_id, uint8_t permissions);
int _mmap(uint64_t region_id, uint64_t address, uint64_t num_pages);
int _munmap(uint64_t region_id, uint64_t address, uint64_t num_pages);

#endif
