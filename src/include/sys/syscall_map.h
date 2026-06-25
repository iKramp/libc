#ifndef LIBC_SYSCALLMAP_H
#define LIBC_SYSCALLMAP_H

#include "stdint.h"

typedef struct {
    uint32_t filesystem_pack;
    uint32_t syscall_pack;
    uint32_t proc_pack;
    uint32_t memory_pack;
    uint32_t namespace_pack;
} MappedSyscallPacks;

MappedSyscallPacks *get_mapped_syscalls();

#endif
