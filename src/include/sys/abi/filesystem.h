#ifndef LIBC_FSABI_H
#define LIBC_FSABI_H

#include <stdint.h>

// OpenFlags:
// uint64_t
// bit 0: read
// bit 1: write
// bit 2: append
// bit 3: truncate
static inline uint64_t make_open_flags(uint8_t read, uint8_t write, uint8_t append, uint8_t truncate) {
    return (read != 0) |
        ((write != 0) << 1) |
        ((append != 0) << 2) |
        ((truncate != 0) << 3);
}
static inline uint8_t is_open_flag_read(uint64_t flags) {
    return (flags & 1) != 0;
}
static inline uint8_t is_open_flag_write(uint64_t flags) {
    return (flags & 2) != 0;
}
static inline uint8_t is_open_flag_append(uint64_t flags) {
    return (flags & 4) != 0;
}
static inline uint8_t is_open_flag_truncate(uint64_t flags) {
    return (flags & 8) != 0;
}

typedef enum {
    START = 0,
    CURRENT = 1,
    END = 2
} SeekMode;

typedef enum {
    FILE = 0,
    DIRECTORY = 1,
    SYMLINK = 2,
    SOCKET = 3,
    BLOCK_DEVICE = 4,
    CHAR_DEVICE = 5,
    FIFO = 6
} InodeType;

// InotePermissionFlags
// uint32_t
// bit 0: other x
// bit 1: other w
// bit 2: other r
// bit 3-5: group rwx
// bit 6-8: user rwx
// bit 9: sticky
// bit 10: sgid
// bit 11: suid

static inline uint32_t make_permission_flags(uint8_t user, uint8_t group, uint8_t other, uint8_t sticky, uint8_t sgid, uint8_t suid) {
    user &= 0b111;
    group &= 0b111;
    other &= 0b111;
    return
        other |
        (group << 3) |
        (user << 6) |
        ((sticky != 0) << 9) |
        ((sgid != 0) << 10) |
        ((suid != 0) << 11);
}

typedef struct {
    uint32_t inner;
} InodeTypeAndPerms;
static inline InodeType get_inode_type(InodeTypeAndPerms tandp) {
    return (InodeType)(tandp.inner >> 24);
}
static inline uint32_t get_inode_permissions(InodeTypeAndPerms tandp) {
    return tandp.inner & 0xFFFFFF;
}

typedef struct {
    uint64_t index;
    uint64_t device;
    InodeTypeAndPerms tandp;
    uint16_t link_cnt;
    uint16_t uid;
    uint16_t gid;
    uint64_t size;
    uint64_t access_time;
    uint64_t modification_time;
    uint64_t stat_change_time;
} InodeStat;

#endif
