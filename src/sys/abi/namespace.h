#ifndef LIBC_ABI_NAMESPACE_H
#define LIBC_ABI_NAMESPACE_H

#include "stdint.h"

typedef enum {
    NAMESPACE_TYPE_SYSCALL = 0,
    NAMESPACE_TYPE_MEMORY = 1,
    NAMESPACE_TYPE_FILESYSTEM = 2,
} NamespaceType;

typedef struct {
    uint64_t id;
    uint32_t namespace_type;
    uint8_t currently_used;
} NamespaceInfo;

#endif
