#include "syscalls/proc.h"
#include "sys/syscall_map.h"
#include "syscalls/syscall_generic.h"

int _exec(ExecArgs *args_ptr) {
    uint64_t syscall_index = get_mapped_syscalls()->proc_pack + 0;
    uint64_t ns = 0;
    syscall_2ret ret = syscall1(syscall_index, ns, (uint64_t)args_ptr);
    return ret.ret0;
}


_Noreturn void _exit(uint64_t exit_code) {
    uint64_t syscall_index = get_mapped_syscalls()->proc_pack + 1;
    uint64_t ns = 0;
    syscall1(syscall_index, ns, exit_code);
    while (1);
}

int _sleep(uint64_t seconds, uint64_t nanoseconds) {
    uint64_t syscall_index = get_mapped_syscalls()->proc_pack + 2;
    uint64_t ns = 0;
    syscall_2ret ret = syscall2(syscall_index, ns, seconds, nanoseconds);
    return ret.ret0;
}

int _getpid() {
    uint64_t syscall_index = get_mapped_syscalls()->proc_pack + 3;
    uint64_t ns = 0;
    syscall_2ret ret = syscall0(syscall_index, ns);
    return ret.ret0;
}

int _set_sig_handler(int sig, void *handler) {
    uint64_t syscall_index = get_mapped_syscalls()->proc_pack + 4;
    uint64_t ns = 0;
    syscall_2ret ret = syscall2(syscall_index, ns, sig, (uint64_t)handler);
    return ret.ret0;
}

int _remove_sig_handler(int sig) {
    uint64_t syscall_index = get_mapped_syscalls()->proc_pack + 5;
    uint64_t ns = 0;
    syscall_2ret ret = syscall1(syscall_index, ns, sig);
    return ret.ret0;
}

int _send_signal(int pid, int sig, int sig_data_len, void *sig_data) {
    uint64_t syscall_index = get_mapped_syscalls()->proc_pack + 6;
    uint64_t ns = 0;
    syscall_2ret ret = syscall4(syscall_index, ns, pid, sig, sig_data_len, (uint64_t)sig_data);
    return ret.ret0;
}
