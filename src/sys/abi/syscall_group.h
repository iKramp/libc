#ifndef LIBC_SYSCALL_GROUP_H
#define LIBC_SYSCALL_GROUP_H

#include "stdint.h"

typedef struct {
    uint8_t name_len;
    uint8_t name[31];
} SyscallGroupInfo;

typedef struct {
    SyscallGroupInfo info;
    uint32_t offset;
    uint32_t mask;
} MappedSyscallGroup;

#endif
