#include "proc.h"
#include "../syscall_map.h"
#include "syscall_generic.h"

int _exec(ExecArgs *args_ptr) {
    uint64_t syscall_index = get_mapped_syscalls()->proc_pack + 0;
    uint64_t ns = 0;
    syscall_2ret ret = syscall1(syscall_index, ns, (uint64_t)args_ptr);
    return ret.ret0;
}


_Noreturn void _exit(uint64_t exit_code) {
    uint64_t syscall_index = get_mapped_syscalls()->proc_pack + 1;
    uint64_t ns = 0;
    syscall_2ret ret = syscall1(syscall_index, ns, exit_code);
    while (1);
}

int _sleep(uint64_t seconds, uint64_t nanoseconds) {
    uint64_t syscall_index = get_mapped_syscalls()->proc_pack + 2;
    uint64_t ns = 0;
    syscall_2ret ret = syscall2(syscall_index, ns, seconds, nanoseconds);
    return ret.ret0;
}
