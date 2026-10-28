#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "limine.h"

void pmm_init(struct limine_memmap_response *memmap, uint64_t hhdm_offset);
void *pmm_alloc_page(void);
void pmm_free_page(void *page);

#endif
