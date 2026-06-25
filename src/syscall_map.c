
#include "internal/sys/syscall_map.h"
#include "sys/abi/syscall_pack.h"
#include "syscalls/syscall.h"

#define SYSCALL_PACK_ARR_CNT 32

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

void init_mapped_syscall_packs() {
    MappedSyscallPacks *mapped_syscalls = get_mapped_syscalls();
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

    if (fs_index == 0xFFFFFFFF || proc_index == 0xFFFFFFFF || syscall_index == 0xFFFFFFFF || memory_index == 0xFFFFFFFF || namespace_index == 0xFFFFFFFF) {
        early_panic();
    }

    mapped_syscalls->filesystem_pack = syscall_pack_arr[fs_index].offset;
    mapped_syscalls->proc_pack = syscall_pack_arr[proc_index].offset;
    mapped_syscalls->syscall_pack = syscall_pack_arr[syscall_index].offset;
    mapped_syscalls->memory_pack = syscall_pack_arr[memory_index].offset;
    mapped_syscalls->namespace_pack = syscall_pack_arr[namespace_index].offset;
}
