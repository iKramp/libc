#ifndef LIBC_SYSCALL_NAMESPACE_H
#define LIBC_SYSCALL_NAMESPACE_H

#include "stdint.h"
#include "include/sys/abi/namespace.h"

typedef struct {
    uint64_t filled_elements;
    uint64_t total_elements;
} BufferFillResult;

int _mknamespace(uint32_t namespace_type, uint64_t existing_id);
int _rmnamespace(uint64_t namespace_id);
int _chnamespace(uint64_t namespace_id);
BufferFillResult _lsnamespace(uint64_t buf_size_elements, NamespaceInfo *buf);

#endif
