#include "include/syscalls/syscall.h"
#include "include/syscalls/syscall_generic.h"
#include "include/sys/syscall_map.h"

int _lsgroups(uint64_t buf_size_elements, MappedSyscallGroup *buf) {
    uint64_t syscall_index = get_mapped_syscalls()->syscall_pack + 0;
    uint64_t ns = 0;
    syscall_2ret ret = syscall2(syscall_index, ns, buf_size_elements, (uint64_t)buf);
    return ret.ret0;
}

int _lsallgroups(uint64_t buf_size_elements, SyscallGroupInfo *buf) {
    uint64_t syscall_index = get_mapped_syscalls()->syscall_pack + 1;
    uint64_t ns = 0;
    syscall_2ret ret = syscall2(syscall_index, ns, buf_size_elements, (uint64_t)buf);
    return ret.ret0;
}

int _mapgroup(uint64_t name_len, uint8_t *name, uint32_t offset) {
    uint64_t syscall_index = get_mapped_syscalls()->syscall_pack + 2;
    uint64_t ns = 0;
    syscall_2ret ret = syscall3(syscall_index, ns, name_len, (uint64_t)name, offset);
    return ret.ret0;
}

int _unmapgroup(uint64_t offset) {
    uint64_t syscall_index = get_mapped_syscalls()->syscall_pack + 3;
    uint64_t ns = 0;
    syscall_2ret ret = syscall1(syscall_index, ns, offset);
    return ret.ret0;
}

int _restrict(uint64_t offset, uint32_t mask) {
    uint64_t syscall_index = get_mapped_syscalls()->syscall_pack + 4;
    uint64_t ns = 0;
    syscall_2ret ret = syscall2(syscall_index, ns, offset, mask);
    return ret.ret0;
}
