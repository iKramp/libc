#include "buddy_allocator.h"
#include "heap.h"
#include "syscalls/proc.h"

struct BuddyAllocator {
    uint64_t n_pages;
    uint64_t allocated_pages;
    uint8_t *bitfield_tree;
    uintptr_t heap_start_addr;
    uintptr_t max_accessed_addr;
};

constexpr uint64_t PAGES = (uint64_t)1 << (HEAP_SIZE_ORDER * 9);
constexpr uint64_t BITFIELD_TREE_ELEMENTS = PAGES * 2;
constexpr uint64_t BITFIELD_TREE_SIZE = (BITFIELD_TREE_ELEMENTS + 7) / 8;
static uint8_t bitfield_tree[BITFIELD_TREE_SIZE];
static struct BuddyAllocator buddy_allocator = {
    .n_pages = PAGES,
    .allocated_pages = 0,
    .bitfield_tree = bitfield_tree,
    .heap_start_addr = 0,
    .max_accessed_addr = 0,
};

void init_buddy_allocator(uintptr_t heap_start) {
    for (uint64_t i = 0; i < BITFIELD_TREE_SIZE; i++) {
        bitfield_tree[i] = 0;
    }
    buddy_allocator.heap_start_addr = heap_start;
    buddy_allocator.max_accessed_addr = heap_start;
    (void)*(volatile uint8_t *)heap_start;
}

void set_at_index(uint64_t index, uint8_t value) {
    uint8_t *byte = &buddy_allocator.bitfield_tree[index / 8];
    if (value) {
        *byte |= (1 << (index % 8));
    } else {
        *byte &= ~(1 << (index % 8));
    }
    buddy_allocator.bitfield_tree[index / 8] = *byte;
}

uint8_t get_at_index(uint64_t index) {
    uint8_t *byte = &buddy_allocator.bitfield_tree[index / 8];
    return (*byte >> (index % 8)) & 1;
}

void update_from_lower(uint64_t index) {
    if (index == 0) {
        return;
    }
    uint8_t all_allocated = get_at_index(index << 1) && get_at_index((index << 1) + 1);
    set_at_index(index, all_allocated);
    update_from_lower(index >> 1);
}

void update_from_higher(uint64_t index, uint8_t value) {
    if (index >= buddy_allocator.n_pages + BITFIELD_TREE_ELEMENTS / 2) {
        return;
    }
    set_at_index(index, value);
    update_from_higher(index << 1, value);
    update_from_higher((index << 1) + 1, value);
}

void mark_index(uint64_t index, uint8_t value) {
    set_at_index(index, value);
    update_from_lower(index >> 1);
    update_from_higher(index << 1, value);
}

void mark_addr(uintptr_t addr, uint8_t value) {
    if (addr & 0xFFF || addr < buddy_allocator.heap_start_addr || addr >= buddy_allocator.heap_start_addr + PAGES * 0x1000) {
        _exit(1);
    }
    addr -= buddy_allocator.heap_start_addr;
    mark_index((addr >> 12) + (BITFIELD_TREE_ELEMENTS / 2), value);
}

uint64_t get_last_level_index(uint64_t node_index) {
    while (node_index < BITFIELD_TREE_ELEMENTS / 2) {
        node_index *= 2;
    }
    return node_index;
}

uint8_t check_all_empty(uint64_t node_index) {
    if (node_index >= BITFIELD_TREE_ELEMENTS / 2) {
        return !get_at_index(node_index);
    }
    return !get_at_index(node_index) && check_all_empty(node_index * 2) && check_all_empty(node_index * 2 + 1);
}

uint64_t find_contiguous_empty_recursively(uint64_t curr_index, uint64_t order) {
    if (curr_index >= BITFIELD_TREE_ELEMENTS / (1 << (order + 1))) {
        if (check_all_empty(curr_index)) {
            return get_last_level_index(curr_index);
        } else {
            return (uint64_t)-1;
        }
    }
    if (!get_at_index(curr_index * 2)) {
        uint64_t res = find_contiguous_empty_recursively(curr_index * 2, order);
        if (res != (uint64_t)-1) {
            return res;
        }
    }
    return find_contiguous_empty_recursively(curr_index * 2 + 1, order);
}

uint64_t find_contiguous_empty(uint64_t n_pages) {
    if (n_pages == 0) {
        return 0;
    }
    uint64_t order = log2_rounded_up(n_pages);
    return find_contiguous_empty_recursively(1, order);
}

void deallocate_page(uint64_t page_addr) {
    mark_addr(page_addr, 0);
    buddy_allocator.allocated_pages--;
}

uint64_t allocate_page(size_t n_pages) {
    if (buddy_allocator.allocated_pages + n_pages > BITFIELD_TREE_ELEMENTS / 2) {
        _exit(1);
    }
    if (n_pages == 0) {
        _exit(1);
    }

    uint64_t index = find_contiguous_empty(n_pages);
    for (uint64_t i = index; i < index + n_pages; i++) {
        mark_index(i, 1);
    }
    buddy_allocator.allocated_pages += n_pages;
    uint64_t address = (index - BITFIELD_TREE_ELEMENTS / 2) * 4096;
    if (address > PAGES * 4096) {
        _exit(1);
    }
    address += buddy_allocator.heap_start_addr;

    for (uint64_t addr = buddy_allocator.max_accessed_addr; addr <= address; addr += 4096) {
        (void)*(volatile uint8_t *)addr; //probe
    }

    return address;
}


//public api

void page_free(uint64_t ptr, size_t n_pages) {
    if (n_pages == 0) {
        return;
    }
    for (size_t i = 0; i < n_pages; i++) {
        deallocate_page((uintptr_t)ptr + (i * 0x1000));
    }
}

uint64_t page_alloc(size_t n_pages) {
    if (n_pages == 0) {
        _exit(1);
    }
    return allocate_page(n_pages);
}
