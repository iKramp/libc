#include "internal/heap/heap.h"
#include "internal/sys/syscall_map.h"
#include "internal/sys/namespace.h"
#include "syscalls/proc.h"
#include <stdint.h>

uint64_t saved_rax;
uint64_t saved_rbx;
uint64_t saved_rcx;
uint64_t saved_rdx;
uint64_t saved_rsi;
uint64_t saved_rdi;
uint64_t saved_rsp;
uint64_t saved_rbp;
uint64_t saved_r8;
uint64_t saved_r9;
uint64_t saved_r10;
uint64_t saved_r11;
uint64_t saved_r12;
uint64_t saved_r13;
uint64_t saved_r14;
uint64_t saved_r15;

extern int main();

void _libc_init() {
    init_mapped_syscall_packs();
    libc_heap_init();
    init_namespaces();
}

void _libc_start() {
    _libc_init();

    int ret = main();

    _exit((uint64_t) ret);
}
