#include "namespace.h"
#include "syscall_generic.h"
#include "../syscall_map.h"

int _mknamespace(uint32_t namespace_type, uint64_t existing_id) {
    uint64_t syscall_index = get_mapped_syscalls()->namespace_pack + 0;
    uint64_t ns = 0;
    syscall_2ret ret = syscall2(syscall_index, ns, namespace_type, existing_id);
    return ret.ret0;
}

int _rmnamespace(uint64_t namespace_id) {
    uint64_t syscall_index = get_mapped_syscalls()->namespace_pack + 1;
    uint64_t ns = 0;
    syscall_2ret ret = syscall1(syscall_index, ns, namespace_id);
    return ret.ret0;
}

int _chnamespace(uint64_t namespace_id) {
    uint64_t syscall_index = get_mapped_syscalls()->namespace_pack + 2;
    uint64_t ns = 0;
    syscall_2ret ret = syscall1(syscall_index, ns, namespace_id);
    return ret.ret0;
}

BufferFillResult _lsnamespace(uint64_t buf_size_elements, NamespaceInfo *buf) {
    uint64_t syscall_index = get_mapped_syscalls()->namespace_pack + 3;
    uint64_t ns = 0;
    syscall_2ret ret = syscall2(syscall_index, ns, buf_size_elements, (uint64_t)buf);
    BufferFillResult result;
    result.filled_elements = ret.ret0;
    result.total_elements = ret.ret1;
    return result;
}
