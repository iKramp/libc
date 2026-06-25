#ifndef LIBC_EXEC_ARGS_H
#define LIBC_EXEC_ARGS_H

#include "stdint.h"

#ifdef __x86_64__
typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rsp;
    uint64_t rbp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
} RegisterState;
#endif

typedef struct {
    uint64_t memory_namespace;
    uint64_t syscall_namespace;
    uint64_t filesystem_namespace;
} NamespaceIds;

typedef struct {
    NamespaceIds namespaces;
    RegisterState registers;
    uint64_t entry_point;
    uint64_t name_len;
    uint8_t *name;
} ExecArgs;

#endif
