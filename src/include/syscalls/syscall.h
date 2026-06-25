#ifndef LIBC_SYSCALL_SYSCALL_H
#define LIBC_SYSCALL_SYSCALL_H

#include "stdint.h"
#include "sys/abi/syscall_pack.h"

int _lsgroups(uint64_t buf_size_elements, MappedSyscallPack *buf);
int _lsallgroups(uint64_t buf_size_elements, SyscallPackInfo *buf);
int _mapgroup(uint64_t name_len, uint8_t *name, uint32_t offset);
int _unmapgroup(uint64_t offset);
int _restrict(uint64_t offset, uint32_t mask);

#endif
