#ifndef LIBC_SYSCALL_TIME_H
#define LIBC_SYSCALL_TIME_H

#include "sys/abi/time_info.h"

int _time(struct TimeInfo *time_info_ptr);
int _set_time_off(uint64_t monotonic_high, uint64_t monotonic_low, uint64_t boot_high, uint64_t boot_low);

#endif
