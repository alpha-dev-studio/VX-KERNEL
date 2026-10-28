#include "vmm.h"
#include "pmm.h"

#define PAGE_SIZE 4096

typedef uint64_t pte_t;

struct page_table {
    pte_t entries[512];
};

static struct page_table *kernel_pml4 = NULL;

void vmm_init(uint64_t hhdm_offset) {
    uint64_t cr3_val;
    __asm__ volatile ("mov %%cr3, %0" : "=r"(cr3_val));

    kernel_pml4 = (struct page_table *)((cr3_val & 0xFFFFF000) + hhdm_offset);
}
