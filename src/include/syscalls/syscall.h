#ifndef LIBC_SYSCALL_SYSCALL_H
#define LIBC_SYSCALL_SYSCALL_H

#include "stdint.h"
#include "include/sys/abi/syscall_group.h"

int _lsgroups(uint64_t buf_size_elements, MappedSyscallGroup *buf);
int _lsallgroups(uint64_t buf_size_elements, SyscallGroupInfo *buf);
int _mapgroup(uint64_t name_len, uint8_t *name, uint32_t offset);
int _unmapgroup(uint64_t offset);
int _restrict(uint64_t offset, uint32_t mask);

#endif
