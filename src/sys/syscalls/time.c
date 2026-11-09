#include "sys/syscall_map.h"
#include "syscalls/syscall_generic.h"
#include "syscalls/time.h"

int _time(struct TimeInfo *time_info_ptr) {
    uint64_t syscall_index = get_mapped_syscalls()->time_pack + 0;
    uint64_t ns = 0;
    syscall_2ret ret = syscall1(syscall_index, ns, (uint64_t)time_info_ptr);
    return ret.ret0;
}
