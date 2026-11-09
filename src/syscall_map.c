#include "internal/sys/syscall_map.h"
#include "sys/abi/syscall_pack.h"
#include "syscalls/syscall.h"
#include <string.h>

MappedSyscallPack syscall_pack_arr[SYSCALL_PACK_ARR_CNT];

MappedSyscallPacks *get_mapped_syscalls() {
    static MappedSyscallPacks mapped_syscalls;
    return &mapped_syscalls;
}

void early_panic(int code) {
    uint8_t *invalid_ptr = (uint8_t *)(0xfffffff000000000 + code * 16); //kernel space
    *invalid_ptr = 0xFF; //write to invalid address to trigger a page fault
}

uint32_t find_index(const char *name, uint32_t name_len, uint32_t entry_count) {
    for (uint32_t i = 0; i < entry_count; i++) {
        if (name_len != syscall_pack_arr[i].info.name_len) {
            continue;
        }
        uint8_t match = strncmp((const char *)syscall_pack_arr[i].info.name, name, name_len);
        if (match == 0) {
            return i;
        }
    }
    return 0xFFFFFFFF; //not found
}

uint32_t map_missing_pack(const char *name, uint32_t name_len, MappedSyscallPacks *mapped_syscalls) {
    uint32_t final_offset = -1;
    for (uint32_t offset_index = 0; offset_index < SYSCALL_PACK_ARR_CNT; offset_index++) {
        uint32_t offset = offset_index * 32; //each pack is 32 bytes
        for (uint32_t checking_pack_index = 0; checking_pack_index < SYSCALL_PACK_ARR_CNT; checking_pack_index++) {
            if (syscall_pack_arr[checking_pack_index].offset == offset) {
                goto next_offset;
            }
        }

        if (mapped_syscalls->filesystem_pack == offset ||
            mapped_syscalls->proc_pack == offset ||
            mapped_syscalls->syscall_pack == offset ||
            mapped_syscalls->memory_pack == offset ||
            mapped_syscalls->namespace_pack == offset) {
            continue;
        }

        final_offset = offset;
        break; //found an unused offset

next_offset:
    }

    if (final_offset == (uint32_t)-1) {
        early_panic(0);
    }

    int ret = _mappacks(name_len, (uint8_t *)name, final_offset);
    if (ret < 0) {
        early_panic(1);
    }
    return final_offset;
}

#define CHECK_PACK(name, mapped_syscalls, pack_field) \
    size_t name##_len = strlen(#name); \
    uint32_t name##_index = find_index(#name, name##_len, SYSCALL_PACK_ARR_CNT); \
    if (name##_index == 0xFFFFFFFF) { \
        mapped_syscalls->pack_field = map_missing_pack(#name, name##_len, mapped_syscalls); \
    } else { \
        mapped_syscalls->pack_field = syscall_pack_arr[name##_index].offset; \
    }

//libc assumes at most SYSCALL_PACK_ARR_CNT packs at program start
void init_mapped_syscall_packs() {
    MappedSyscallPacks *mapped_syscalls = get_mapped_syscalls();
    mapped_syscalls->filesystem_pack = 0xFFFFFFFF; //invalid
    mapped_syscalls->proc_pack = 0xFFFFFFFF; //invalid
    mapped_syscalls->memory_pack = 0xFFFFFFFF; //invalid
    mapped_syscalls->namespace_pack = 0xFFFFFFFF; //invalid
    mapped_syscalls->time_pack = 0xFFFFFFFF; //invalid

    mapped_syscalls->syscall_pack = 0xFFFFFFE0; //highest valid pack offset - standard
    int ret = _lspacks(SYSCALL_PACK_ARR_CNT, syscall_pack_arr);
    if (ret < 0) {
        early_panic(3);
    }

    CHECK_PACK(fs, mapped_syscalls, filesystem_pack);
    CHECK_PACK(proc, mapped_syscalls, proc_pack);
    CHECK_PACK(syscall_management, mapped_syscalls, syscall_pack);
    CHECK_PACK(memory, mapped_syscalls, memory_pack);
    CHECK_PACK(namespace_management, mapped_syscalls, namespace_pack);
    CHECK_PACK(time, mapped_syscalls, time_pack);
}
