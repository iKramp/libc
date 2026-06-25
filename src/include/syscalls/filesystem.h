#ifndef LIBC_SYSCALL_FS_H
#define LIBC_SYSCALL_FS_H

#include "include/sys/abi/filesystem.h"

int _fopen(uint64_t path_len, const char* path_ptr, uint64_t, uint64_t flags);
int _fclose(uint64_t fd);
int _fread(uint64_t fd, uint64_t count, void* buf);
int _fwrite(uint64_t fd, uint64_t count, const void* buf);
int _fseek(uint64_t fd, int64_t offset, uint64_t mode);
int _fcreate(uint64_t name_len, const char* name_ptr, uint64_t parent_fd, uint64_t type, uint64_t permissions);
int _flink(uint64_t name_len, const char* name_ptr, uint64_t parent_fd, uint64_t target_fd);
int _funlink(uint64_t fd, uint64_t name_len, const char* name_ptr);
int _fstat(uint64_t fd, InodeStat *stat_ptr);

#endif
