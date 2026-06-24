#include "filesystem.h"
#include "syscall_generic.h"
#include "../syscall_map.h"

int _fopen(uint64_t path_len, const char* path_ptr, uint64_t fd, uint64_t flags) {
    uint64_t syscall_index = get_mapped_syscalls()->filesystem_pack + 0;
    uint64_t ns = 0;
    syscall_2ret ret = syscall4(syscall_index, ns, path_len, (uint64_t)path_ptr, fd, flags);
    return ret.ret0;
}

int _fclose(uint64_t fd) {
    uint64_t syscall_index = get_mapped_syscalls()->filesystem_pack + 1;
    uint64_t ns = 0;
    syscall_2ret ret = syscall1(syscall_index, ns, fd);
    return ret.ret0;
}

int _fread(uint64_t fd, uint64_t count, void* buf) {
    uint64_t syscall_index = get_mapped_syscalls()->filesystem_pack + 2;
    uint64_t ns = 0;
    syscall_2ret ret = syscall3(syscall_index, ns, fd, count, (uint64_t)buf);
    return ret.ret0;
}

int _fwrite(uint64_t fd, uint64_t count, const void* buf) {
    uint64_t syscall_index = get_mapped_syscalls()->filesystem_pack + 3;
    uint64_t ns = 0;
    syscall_2ret ret = syscall3(syscall_index, ns, fd, count, (uint64_t)buf);
    return ret.ret0;
}

int _fseek(uint64_t fd, int64_t offset, uint64_t mode) {
    uint64_t syscall_index = get_mapped_syscalls()->filesystem_pack + 4;
    uint64_t ns = 0;
    syscall_2ret ret = syscall3(syscall_index, ns, fd, (uint64_t)offset, mode);
    return ret.ret0;
}

int _fcreate(uint64_t name_len, const char* name_ptr, uint64_t parent_fd, uint64_t type, uint64_t permissions) {
    uint64_t syscall_index = get_mapped_syscalls()->filesystem_pack + 5;
    uint64_t ns = 0;
    syscall_2ret ret = syscall4(syscall_index, ns, name_len, (uint64_t)name_ptr, parent_fd, type);
    return ret.ret0;
}

int _flink(uint64_t name_len, const char* name_ptr, uint64_t parent_fd, uint64_t target_fd) {
    uint64_t syscall_index = get_mapped_syscalls()->filesystem_pack + 6;
    uint64_t ns = 0;
    syscall_2ret ret = syscall4(syscall_index, ns, name_len, (uint64_t)name_ptr, parent_fd, target_fd);
    return ret.ret0;
}

int _funlink(uint64_t fd, uint64_t name_len, const char* name_ptr) {
    uint64_t syscall_index = get_mapped_syscalls()->filesystem_pack + 7;
    uint64_t ns = 0;
    syscall_2ret ret = syscall3(syscall_index, ns, fd, name_len, (uint64_t)name_ptr);
    return ret.ret0;
}

int _fstat(uint64_t fd, InodeStat *stat_ptr) {
    uint64_t syscall_index = get_mapped_syscalls()->filesystem_pack + 8;
    uint64_t ns = 0;
    syscall_2ret ret = syscall2(syscall_index, ns, fd, (uint64_t)stat_ptr);
    return ret.ret0;
}
