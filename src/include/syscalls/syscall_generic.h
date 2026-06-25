#ifndef LIBC_SYSCALL_GENERIC_H
#define LIBC_SYSCALL_GENERIC_H

#include <stdint.h>

typedef struct {
    int ret0;
    int ret1;
} syscall_2ret; //most standard return ig?

syscall_2ret syscall0(uint64_t syscall_index, uint64_t namespace_index);
syscall_2ret syscall1(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0);
syscall_2ret syscall2(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0, uint64_t arg1);
syscall_2ret syscall3(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0, uint64_t arg1, uint64_t arg2);
syscall_2ret syscall4(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3);
syscall_2ret syscall5(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);
syscall_2ret syscall6(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
syscall_2ret syscall7(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6);

#endif
