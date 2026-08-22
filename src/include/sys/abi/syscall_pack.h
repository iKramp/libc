#ifndef LIBC_SYSCALL_PACK_H
#define LIBC_SYSCALL_PACK_H

#include "stdint.h"

#define SYSCALL_PACK_ARR_CNT 32

typedef struct {
    uint8_t name_len;
    uint8_t name[31];
} SyscallPackInfo;

typedef struct {
    SyscallPackInfo info;
    uint32_t offset;
    uint32_t mask;
} MappedSyscallPack;

#endif
