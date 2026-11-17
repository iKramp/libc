#include "extra.h"
#include "sys/abi/exec_args.h"
#include "syscalls/proc.h"
#include <string.h>

int clone() {
    char *name = "cloned proc, no name";
    uint64_t name_len = strlen(name);

    NamespaceIds ns_ids = {.memory_namespace = 0,
                           .syscall_namespace = 0,
                           .filesystem_namespace = 0,
                           .time_namespace = 0,
                           .mount_namespace = 0};
    ExecArgs exec_args = {.namespaces = ns_ids,
                           .registers = {0},
                           .entry_point = 0,
                           .name_len = name_len,
                           .name = (uint8_t *)name,
                           .clone_ns_table = 1};
    
    int is_child = 0;

    //intel syntax
    __asm__ volatile (
        "mov rdx, child_continue\n"
        "mov [rsi], rdx\n"
        "mov [rdi], rax\n"
        "mov [rdi + 8*1], rbx\n"
        "mov [rdi + 8*2], rcx\n"
        "mov [rdi + 8*3], rdx\n"
        "mov [rdi + 8*4], rsi\n"
        "mov [rdi + 8*5], rdi\n"
        "mov [rdi + 8*6], rsp\n"
        "mov [rdi + 8*7], rbp\n"
        "mov [rdi + 8*8], r8\n"
        "mov [rdi + 8*9], r9\n"
        "mov [rdi + 8*10], r10\n"
        "mov [rdi + 8*11], r11\n"
        "mov [rdi + 8*12], r12\n"
        "mov [rdi + 8*13], r13\n"
        "mov [rdi + 8*14], r14\n"
        "mov [rdi + 8*15], r15\n"
        "jmp parent_continue\n"


        "child_continue:\n"
        "mov rax, 1\n" // set rax to 1 for child
        "jmp end_clone\n"

        "parent_continue:\n"
        "mov rax, 0\n" // set rax to 0 for parent

        "end_clone:\n"

        : "=a"(is_child) // output to rax
        : "D"(&exec_args.registers), "S"(&exec_args.entry_point) // input registers ptr to rdi, entry point ptr to rsi
        : "memory", "rdx"
    );

    if (is_child) {
        return 0;
    }

    int child_pid = _exec(&exec_args);
    return child_pid;
}
