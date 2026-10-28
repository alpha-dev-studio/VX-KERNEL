#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void pmm_init(void *memmap, uint64_t hhdm_offset);
void *pmm_alloc_page(void);
void pmm_free_page(void *page);

#endif
