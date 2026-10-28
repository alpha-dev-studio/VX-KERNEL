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

void pmm_init(struct limine_memmap_response *memmap, uint64_t hhdm_offset) {
    uint64_t highest_address = 0;

    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE) {
            uint64_t top = entry->base + entry->length;
            if (top > highest_address) {
                highest_address = top;
            }
        }
    }

    total_memory_pages = highest_address / PAGE_SIZE;
    bitmap_size = (total_memory_pages + 7) / 8;

    // Find a usable entry for bitmap and access via HHDM
    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE && entry->length >= bitmap_size) {
            bitmap = (uint8_t *)(entry->base + hhdm_offset);
            for (uint64_t b = 0; b < bitmap_size; b++) {
                bitmap[b] = 0xFF; // Mark all as used
            }
            break;
        }
    }

    // Mark usable regions as free
    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE) {
            uint64_t start_page = entry->base / PAGE_SIZE;
            uint64_t page_count = entry->length / PAGE_SIZE;
            for (uint64_t p = 0; p < page_count; p++) {
                bitmap_clear(start_page + p);
            }
        }
    }

    // Mark page 0 and bitmap pages as used
    bitmap_set(0);
    uint64_t bitmap_phys = (uint64_t)bitmap - hhdm_offset;
    uint64_t bitmap_start_page = bitmap_phys / PAGE_SIZE;
    uint64_t bitmap_page_count = (bitmap_size + PAGE_SIZE - 1) / PAGE_SIZE;
    for (uint64_t p = 0; p < bitmap_page_count; p++) {
        bitmap_set(bitmap_start_page + p);
    }
}

void *pmm_alloc_page(void) {
    for (uint64_t i = last_free_page; i < total_memory_pages; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            last_free_page = i;
            return (void *)(i * PAGE_SIZE);
        }
    }

    for (uint64_t i = 0; i < last_free_page; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            last_free_page = i;
            return (void *)(i * PAGE_SIZE);
        }
    }

    return NULL;
}

void pmm_free_page(void *page) {
    uint64_t page_addr = (uint64_t)page;
    uint64_t bit = page_addr / PAGE_SIZE;
    bitmap_clear(bit);
    if (bit < last_free_page) {
        last_free_page = bit;
    }
}
