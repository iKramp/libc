#ifndef LIBC_SYSCALL_PROC_H
#define LIBC_SYSCALL_PROC_H

#include "stdint.h"
#include "include/sys/abi/exec_args.h"

int _exec(ExecArgs *args_ptr);
_Noreturn void _exit(uint64_t exit_code);
int _sleep(uint64_t seconds, uint64_t nanoseconds);

#endif
