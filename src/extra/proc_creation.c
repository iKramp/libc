#include "extra.h"
#include "sys/abi/exec_args.h"
#include "sys/abi/namespace.h"
#include "syscalls/namespace.h"
#include "syscalls/proc.h"
#include <stdlib.h>
#include <string.h>



int clone() {
    exit(561); //clone the stack
    char *name = "cloned proc, no name";
    uint64_t name_len = strlen(name);

    NamespaceIds ns_ids = {.memory_namespace = 0,
                           .syscall_namespace = 0,
                           .open_file_namespace = 0,
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
        "mov rdx, child_continue_clone\n"
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
        "jmp parent_continue_clone\n"


        "child_continue_clone:\n"
        "mov rax, 1\n" // set rax to 1 for child
        "jmp end_clone\n"

        "parent_continue_clone:\n"
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

int clone_namespaces_for_fork(NamespaceIds *target) {

    int mem_ns = _mknamespace(NAMESPACE_TYPE_MEMORY, 0, false);
    int syscall_ns = _mknamespace(NAMESPACE_TYPE_SYSCALL, 0, false);
    int open_file_ns = _mknamespace(NAMESPACE_TYPE_OPEN_FILE, 0, false);
    //keep time and mount shared
    if (mem_ns < 0 || syscall_ns < 0 || open_file_ns < 0) {
        if (mem_ns >= 0) {
            _rmnamespace(mem_ns);
        }
        if (syscall_ns >= 0) {
            _rmnamespace(syscall_ns);
        }
        if (open_file_ns >= 0) {
            _rmnamespace(open_file_ns);
        }
        return -1;
    }
    target->memory_namespace = mem_ns;
    target->syscall_namespace = syscall_ns;
    target->open_file_namespace = open_file_ns;
    target->time_namespace = 0;
    target->mount_namespace = 0;
    return 0;
}


int fork() {
    char *name = "cloned proc, no name";
    uint64_t name_len = strlen(name);

    NamespaceIds ns_ids;

    if (clone_namespaces_for_fork(&ns_ids) < 0) {
        return -1;
    }

    ExecArgs exec_args = {.namespaces = ns_ids,
                           .registers = {0},
                           .entry_point = 0,
                           .name_len = name_len,
                           .name = (uint8_t *)name,
                           .clone_ns_table = 1};
    
    int is_child = 0;

    //intel syntax
    __asm__ volatile (
        "movabs rdx, OFFSET FLAT:child_continue_fork\n"
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
        "jmp parent_continue_fork\n"


        "child_continue_fork:\n"
        "mov rax, 1\n" // set rax to 1 for child
        "jmp end_fork\n"

        "parent_continue_fork:\n"
        "mov rax, 0\n" // set rax to 0 for parent

        "end_fork:\n"

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
