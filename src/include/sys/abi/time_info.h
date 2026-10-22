#ifndef LIBC_TIME_INFO_H
#define LIBC_TIME_INFO_H

#include "stdint.h"

struct Duration {
    uint64_t seconds;
    uint64_t nanoseconds;
};

struct TimeInfo {
    struct Duration time_since_epoch;
    struct Duration time_since_boot;
    struct Duration program_start_time;
    struct Duration program_userspace_cpu_time;
    struct Duration program_kernelspace_cpu_time;
};

#endif
