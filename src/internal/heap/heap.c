#include "heap.h"
#include "internal/heap/buddy_allocator.h"
#include "syscalls/memory.h"
#include "syscalls/proc.h"
#include "syscalls/syscall_generic.h"
#include "stdint.h"
#include "stddef.h"

//min allocation is 16 bytes
//16
//32
//64
//128
//256
//512
//1024
//above that we allocate whole pages

static void *heap_start = NULL;
static uint64_t heap_region_id = 0;

struct HeapPageMetadata {
    uint8_t size_order_of_objects;
    uint8_t number_of_allocations;
    uint8_t max_allocations;
    uint64_t ptr_to_first;
    uint64_t ptr_to_last;
};

struct EmptyBlock {
    uint64_t ptr_to_prev;
    uint64_t ptr_to_next;
};

struct HeapAllocationData {
    uint8_t size_order_of_objects;
    uint64_t free_objects;
    uint64_t ptr_to_first;
};
static struct HeapAllocationData HEAP[7];

void populate_heap_page(struct HeapPageMetadata *metadata, uintptr_t page_addr) {
    uint64_t size_of_object = 1 << metadata->size_order_of_objects;
    uint64_t addr_of_first = page_addr = (4096 - size_of_object * metadata->max_allocations);
    for (uint64_t i = addr_of_first; i < page_addr + 4096; i += size_of_object) {
        struct EmptyBlock *empty_block = (struct EmptyBlock*)i;
        empty_block->ptr_to_prev = i - size_of_object;
        empty_block->ptr_to_next = i + size_of_object;
    }
    ((struct EmptyBlock *)addr_of_first)->ptr_to_prev = page_addr + 4096 - size_of_object;
    ((struct EmptyBlock *)page_addr + 4096 - size_of_object)->ptr_to_next = addr_of_first;
    metadata->ptr_to_first = addr_of_first;
    metadata->ptr_to_last = page_addr + 4096 - size_of_object;
}

uint64_t HeapAllocationData__allocate(struct HeapAllocationData *self) {
    if (self->free_objects == 0) {
        uint64_t new_page = page_alloc(1);
        if (new_page == (uint64_t)-1) {
            _exit(1);
        }

        struct HeapPageMetadata metadata;
        metadata.size_order_of_objects = self->size_order_of_objects;
        metadata.number_of_allocations = 0;
        metadata.max_allocations = ((4096 - sizeof(struct HeapPageMetadata)) / (2 << self->size_order_of_objects));
        populate_heap_page(&metadata, new_page);
        *(struct HeapPageMetadata *)new_page = metadata;
        self->free_objects = metadata.max_allocations;
        self->ptr_to_first = metadata.ptr_to_first;
    }

    uint64_t allocated = self->ptr_to_first;

    struct HeapPageMetadata *page_metadata = (struct HeapPageMetadata *)(self->ptr_to_first & !0xFFF);
    page_metadata->number_of_allocations += 1;
    self->free_objects -= 1;

    if (page_metadata->number_of_allocations < page_metadata->max_allocations) {
        struct EmptyBlock *empty_block = (struct EmptyBlock *)allocated;
        uint64_t after = empty_block->ptr_to_next;
        page_metadata->ptr_to_first = after;
    }

    if (self->free_objects > 0) {
        struct EmptyBlock *empty_block = (struct EmptyBlock *)allocated;
        uint64_t before = empty_block->ptr_to_prev;
        uint64_t after = empty_block->ptr_to_next;
        struct EmptyBlock *before_block = (struct EmptyBlock *)before;
        struct EmptyBlock *after_block = (struct EmptyBlock *)after;
        self->ptr_to_first = after;
        before_block->ptr_to_next = after;
        after_block->ptr_to_prev = before;
    }
    return allocated;
}

void HeapAllocationData__deallocate(struct HeapAllocationData *self, uint64_t addr) {
    struct HeapPageMetadata *metadata = (struct HeapPageMetadata *)(addr & !0xFFF);

    uint8_t no_empty_cells = self->free_objects == 0;
    uint8_t full_frame = metadata->max_allocations == metadata->number_of_allocations;

    if (no_empty_cells) {
        self->ptr_to_first = addr;
        struct EmptyBlock new_block = {
            .ptr_to_next = addr,
            .ptr_to_prev = addr
        };
        *(struct EmptyBlock *)addr = new_block;
        metadata->ptr_to_first = addr;
        metadata->ptr_to_last = addr;
        metadata->number_of_allocations -= 1;
        self->free_objects += 1;
        return;
    }

    struct EmptyBlock *last_block, *past_last_block;
    if (full_frame) {
        past_last_block = (struct EmptyBlock *)self->ptr_to_first;
        last_block = (struct EmptyBlock *)past_last_block->ptr_to_prev;
    } else {
        last_block = (struct EmptyBlock *)metadata->ptr_to_last;
        past_last_block = (struct EmptyBlock *)last_block->ptr_to_next;
    }

    metadata->number_of_allocations -= 1;
    self->free_objects += 1;

    struct EmptyBlock newBlock = {
        .ptr_to_next = last_block->ptr_to_next,
        .ptr_to_prev = metadata->ptr_to_last
    };
    *(struct EmptyBlock *)addr = newBlock;
    last_block->ptr_to_next = addr;
    past_last_block->ptr_to_prev = addr;
    metadata->ptr_to_last = addr;
}

void libc_heap_init() {
    uint64_t addr = 0;
    uint8_t order = HEAP_SIZE_ORDER; //max 512GB heap
    uint8_t permissions = 0b01; //no execute, write
    uint8_t region_type = 2; //heap
    uint64_t management_mode = 0; //managed by kernel, growing up
    uint64_t region_name_len = 4;
    uint8_t *region_name = (uint8_t *) "heap";
    syscall_2ret ret = _make_region(addr, order, permissions, region_type, management_mode, region_name_len, region_name);

    if (ret.ret0 == (uint64_t)-1) {
        _exit(1);
    }

    heap_region_id = ret.ret0;
    heap_start = (void *) ret.ret1;
    init_buddy_allocator((uintptr_t)heap_start);

    //can start actually using heap

    for (int i = 0; i < 7; i++) {
        HEAP[i].free_objects = 0;
    }

    HEAP[0].size_order_of_objects = 4;
    HEAP[1].size_order_of_objects = 5;
    HEAP[2].size_order_of_objects = 6;
    HEAP[3].size_order_of_objects = 7;
    HEAP[4].size_order_of_objects = 8;
    HEAP[5].size_order_of_objects = 9;
    HEAP[6].size_order_of_objects = 10;
}

void *malloc(size_t size) {
    if (size == 0) {
        return (void *)&HEAP;
    } else if (size > 1024) {
        uint64_t original_size = size;
        size += 8; //metadata
        uint64_t n_of_pages = (size + 4095) / 4096;
        
        uint64_t page_addr = page_alloc(n_of_pages);
        *(uint64_t*) page_addr = original_size | ((uint64_t)1 << 63);
        return (void *)(page_addr + 8);
    } else {
        uint64_t size_order = log2_rounded_up(size);
        uint64_t index = (size_order < 4 ? 4 : size_order) - 4;       
        return (void *)HeapAllocationData__allocate(&HEAP[index]);
    }
}

void free(void *ptr) {
    if (ptr == (void *)&HEAP) {
        //size 0
        return;
    }

    uint64_t page_addr = (uint64_t)ptr & !0xFFF;
    uint64_t first_qword = *(uint64_t *)page_addr;
    if ((first_qword & ((uint64_t)1 << 63)) != 0) {
        //page
        uint64_t original_size = first_qword & !((uint64_t)1 << 63);
        uint64_t size = original_size + 8;
        uint64_t n_of_pages = (size + 4095) / 4096;
        page_free(((uint64_t)ptr) - 8, n_of_pages);
    } else {
        struct HeapPageMetadata *metadata = (struct HeapPageMetadata *)page_addr;
        uint64_t index = metadata->size_order_of_objects - 4;
        HeapAllocationData__deallocate(&HEAP[index], (uint64_t)ptr);
    }
}

uint64_t log2_rounded_up(uint64_t num) {
    unsigned n = 0;

    if (num <= 1)
        return 0;

    num--;
    while (num >>= 1)
        n++;

    return n + 1;
}

