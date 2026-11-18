#ifndef LIBC_SYSCALL_PROC_H
#define LIBC_SYSCALL_PROC_H

#include "stdint.h"
#include "sys/abi/exec_args.h"

int _exec(ExecArgs *args_ptr);
_Noreturn void _exit(uint64_t exit_code);
int _sleep(uint64_t seconds, uint64_t nanoseconds);
int _getpid();
int _set_sig_handler(int sig, void *handler);
int _remove_sig_handler(int sig);
int _send_signal(int pid, int sig, int sig_data_len, void *sig_data);

#endif
