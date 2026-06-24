#include "syscall_generic.h"

syscall_2ret syscall0(uint64_t syscall_index, uint64_t namespace_index) {
    uint64_t ret0, ret1;
    __asm__ volatile (
        "syscall"
        : "=d"(ret0), "=D"(ret1)
        : "a"(syscall_index), "b"(namespace_index)
        : "memory"
    );
    syscall_2ret ret;
    ret.ret0 = ret0;
    ret.ret1 = ret1;
    return ret;
}

syscall_2ret syscall1(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0) {
    uint64_t ret0, ret1;
    __asm__ volatile (
        "syscall"
        : "=d"(ret0), "=D"(ret1)
        : "a"(syscall_index), "b"(namespace_index), "d"(arg0)
        : "memory"
    );
    syscall_2ret ret;
    ret.ret0 = ret0;
    ret.ret1 = ret1;
    return ret;
}

syscall_2ret syscall2(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0, uint64_t arg1) {
    uint64_t ret0, ret1;
    __asm__ volatile (
        "syscall"
        : "=d"(ret0), "=D"(ret1)
        : "a"(syscall_index), "b"(namespace_index), "d"(arg0), "D"(arg1)
        : "memory"
    );
    syscall_2ret ret;
    ret.ret0 = ret0;
    ret.ret1 = ret1;
    return ret;
}

syscall_2ret syscall3(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0, uint64_t arg1, uint64_t arg2) {
    uint64_t ret0, ret1;
    __asm__ volatile (
        "syscall"
        : "=d"(ret0), "=D"(ret1)
        : "a"(syscall_index), "b"(namespace_index), "d"(arg0), "D"(arg1), "s"(arg2)
        : "memory"
    );
    syscall_2ret ret;
    ret.ret0 = ret0;
    ret.ret1 = ret1;
    return ret;
}

syscall_2ret syscall4(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t ret0, ret1;

    register uint64_t r8 asm("r8") = arg3;

    __asm__ volatile (
        "syscall"
        : "=d"(ret0), "=D"(ret1)
        : "a"(syscall_index), "b"(namespace_index), "d"(arg0), "D"(arg1), "s"(arg2), "r"(r8)
        : "memory"
    );
    syscall_2ret ret;
    ret.ret0 = ret0;
    ret.ret1 = ret1;
    return ret;
}

syscall_2ret syscall5(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t ret0, ret1;

    register uint64_t r8 asm("r8") = arg3;
    register uint64_t r9 asm("r8") = arg4;

    __asm__ volatile (
        "syscall"
        : "=d"(ret0), "=D"(ret1)
        : "a"(syscall_index), "b"(namespace_index), "d"(arg0), "D"(arg1), "s"(arg2), "r"(r8), "r"(r9)
        : "memory"
    );
    syscall_2ret ret;
    ret.ret0 = ret0;
    ret.ret1 = ret1;
    return ret;
}

syscall_2ret syscall6(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    uint64_t ret0, ret1;

    register uint64_t r8 asm("r8") = arg3;
    register uint64_t r9 asm("r9") = arg4;
    register uint64_t r10 asm("r10") = arg5;

    __asm__ volatile (
        "syscall"
        : "=d"(ret0), "=D"(ret1)
        : "a"(syscall_index), "b"(namespace_index), "d"(arg0), "D"(arg1), "s"(arg2), "r"(r8), "r"(r9), "r"(r10)
        : "memory"
    );
    syscall_2ret ret;
    ret.ret0 = ret0;
    ret.ret1 = ret1;
    return ret;
}

syscall_2ret syscall7(uint64_t syscall_index, uint64_t namespace_index, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
    uint64_t ret0, ret1;

    register uint64_t r8 asm("r8") = arg3;
    register uint64_t r9 asm("r9") = arg4;
    register uint64_t r10 asm("r10") = arg5;
    register uint64_t r11 asm("r11") = arg6;

    __asm__ volatile (
        "syscall"
        : "=d"(ret0), "=D"(ret1)
        : "a"(syscall_index), "b"(namespace_index), "d"(arg0), "D"(arg1), "s"(arg2), "r"(r8), "r"(r9), "r"(r10), "r"(r11)
        : "memory"
    );
    syscall_2ret ret;
    ret.ret0 = ret0;
    ret.ret1 = ret1;
    return ret;
}
