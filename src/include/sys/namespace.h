#ifndef LIBC_NAMESPACE_H
#define LIBC_NAMESPACE_H

#include "stdint.h"

typedef struct {
    uint32_t syscall;
    uint32_t memory;
    uint32_t filesystem;
} CurrNamespaces;

#define NAMESPACE_TYPE_SYSCALL 0
#define NAMESPACE_TYPE_MEMORY 1
#define NAMESPACE_TYPE_FILESYSTEM 2

CurrNamespaces *get_curr_namespaces();

#endif
