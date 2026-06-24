#include "memory.h"
#include "syscall_generic.h"
#include "../syscall_map.h"

int _make_region(uint64_t address, uint8_t order, uint8_t permissions,
                 uint8_t region_type, uint64_t management_mode,
                 uint64_t name_len, uint8_t *name_ptr) {
    uint64_t syscall_index = get_mapped_syscalls()->memory_pack + 0;
    uint64_t ns = 0;
    syscall_2ret ret = syscall7(syscall_index, ns, address, order, permissions, region_type, management_mode, name_len, (uint64_t)name_ptr);
    return ret.ret0;
}

int _remove_region(uint64_t region_id) {
    uint64_t syscall_index = get_mapped_syscalls()->memory_pack + 1;
    uint64_t ns = 0;
    syscall_2ret ret = syscall1(syscall_index, ns, region_id);
    return ret.ret0;
}

int _set_protection(uint64_t region_id, uint8_t permissions) {
    uint64_t syscall_index = get_mapped_syscalls()->memory_pack + 2;
    uint64_t ns = 0;
    syscall_2ret ret = syscall2(syscall_index, ns, region_id, permissions);
    return ret.ret0;
}

int _mmap(uint64_t region_id, uint64_t address, uint64_t num_pages) {
    uint64_t syscall_index = get_mapped_syscalls()->memory_pack + 3;
    uint64_t ns = 0;
    syscall_2ret ret = syscall3(syscall_index, ns, region_id, address, num_pages);
    return ret.ret0;
}

int _munmap(uint64_t region_id, uint64_t address, uint64_t num_pages) {
    uint64_t syscall_index = get_mapped_syscalls()->memory_pack + 4;
    uint64_t ns = 0;
    syscall_2ret ret = syscall3(syscall_index, ns, region_id, address, num_pages);
    return ret.ret0;
}
