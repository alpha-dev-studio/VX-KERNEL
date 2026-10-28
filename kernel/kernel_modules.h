#ifndef KERNEL_MODULES_H
#define KERNEL_MODULES_H

#include <stdint.h>
#include <stdbool.h>

struct kernel_module {
    char name[64];
    uint64_t base_address;
    uint64_t size;
    bool loaded;
};

void kernel_modules_init(void);
bool module_load(const char *name, uint64_t addr, uint64_t size);
bool module_unload(const char *name);

#endif
