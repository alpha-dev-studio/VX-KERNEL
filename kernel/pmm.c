#include "pmm.h"

static uint8_t *bitmap = NULL;
static uint64_t bitmap_size = 0;
static uint64_t total_memory_pages = 0;
static uint64_t last_free_page = 0;

#define PAGE_SIZE 4096

static void bitmap_set(uint64_t bit) {
    bitmap[bit / 8] |= (1 << (bit % 8));
}

static void bitmap_clear(uint64_t bit) {
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

static bool bitmap_test(uint64_t bit) {
    return (bitmap[bit / 8] & (1 << (bit % 8))) != 0;
}

void pmm_init(void *memmap, uint64_t hhdm_offset) {
    (void)memmap;
    (void)hhdm_offset;

    // Without Limine boot info, we cannot discover physical memory layout.
    // PMM remains uninitialized until add-limine.sh provides boot info.
    total_memory_pages = 0;
    bitmap_size = 0;
    bitmap = NULL;
}

void *pmm_alloc_page(void) {
    (void)bitmap;
    (void)total_memory_pages;
    (void)last_free_page;
    return NULL;
}

void pmm_free_page(void *page) {
    (void)page;
}
