
#include "internal/sys/syscall_map.h"
#include "sys/abi/syscall_pack.h"
#include "syscalls/syscall.h"

MappedSyscallPack syscall_pack_arr[SYSCALL_PACK_ARR_CNT];

MappedSyscallPacks *get_mapped_syscalls() {
    static MappedSyscallPacks mapped_syscalls;
    return &mapped_syscalls;
}

void early_panic() {
    uint8_t *invalid_ptr = (uint8_t *)0xFFFFFFFFFFFFFFFF; //kernel space
    *invalid_ptr = 0xFF; //write to invalid address to trigger a page fault
}

uint32_t find_index(const char *name, uint32_t name_len, uint32_t entry_count) {
    for (uint32_t i = 0; i < entry_count; i++) {
        for (uint32_t j = 0; j < name_len; j++) {
            if (syscall_pack_arr[i].info.name[j] != (uint8_t)name[j]) {
                break;
            }
            if (j == name_len - 1) {
                return i;
            }
        }
    }
    return 0xFFFFFFFF; //not found
}

uint32_t map_missing_pack(const char *name, uint32_t name_len) {
    uint32_t final_offset = -1;
    for (uint32_t offset_index = 0; offset_index < SYSCALL_PACK_ARR_CNT; offset_index++) {
        uint32_t offset = offset_index * 32; //each pack is 32 bytes
        for (uint32_t checking_pack_index = 0; checking_pack_index < SYSCALL_PACK_ARR_CNT; checking_pack_index++) {
            if (syscall_pack_arr[checking_pack_index].offset == offset) {
                goto next_offset;
            }
        }
        final_offset = offset;
        break; //found an unused offset

next_offset:
    }

    if (final_offset == (uint32_t)-1) {
        early_panic();
    }

    int ret = _mapgroup(name_len, (uint8_t *)name, final_offset);
    if (ret < 0) {
        early_panic();
    }
    return final_offset;
}

//libc assumes at most SYSCALL_PACK_ARR_CNT packs at program start
void init_mapped_syscall_packs() {
    MappedSyscallPacks *mapped_syscalls = get_mapped_syscalls();
    mapped_syscalls->filesystem_pack = 0xFFFFFFFF; //invalid
    mapped_syscalls->proc_pack = 0xFFFFFFFF; //invalid
    mapped_syscalls->memory_pack = 0xFFFFFFFF; //invalid
    mapped_syscalls->namespace_pack = 0xFFFFFFFF; //invalid

    mapped_syscalls->syscall_pack = 0xFFFFFFE0; //highest valid pack offset - standard
    int ret = _lsgroups(SYSCALL_PACK_ARR_CNT, syscall_pack_arr);
    if (ret < 0) {
        early_panic();
    }

    int num_packs = ret;

    uint32_t fs_index = find_index("fs", 2, num_packs);
    uint32_t proc_index = find_index("proc", 4, num_packs);
    uint32_t syscall_index = find_index("syscall_management", 18, num_packs);
    uint32_t memory_index = find_index("memory", 6, num_packs);
    uint32_t namespace_index = find_index("namespace_management", 20, num_packs);

    if (fs_index == 0xFFFFFFFF) {
        fs_index = map_missing_pack("fs", 2);
    }
    if (proc_index == 0xFFFFFFFF) {
        proc_index = map_missing_pack("proc", 4);
    }
    if (syscall_index == 0xFFFFFFFF) {
        syscall_index = map_missing_pack("syscall_management", 18);
    }
    if (memory_index == 0xFFFFFFFF) {
        memory_index = map_missing_pack("memory", 6);
    }
    if (namespace_index == 0xFFFFFFFF) {
        namespace_index = map_missing_pack("namespace_management", 20);
    }

    mapped_syscalls->filesystem_pack = syscall_pack_arr[fs_index].offset;
    mapped_syscalls->proc_pack = syscall_pack_arr[proc_index].offset;
    mapped_syscalls->syscall_pack = syscall_pack_arr[syscall_index].offset;
    mapped_syscalls->memory_pack = syscall_pack_arr[memory_index].offset;
    mapped_syscalls->namespace_pack = syscall_pack_arr[namespace_index].offset;
}
