#include "kernel_modules.h"

#define MAX_MODULES 32
static struct kernel_module loaded_modules[MAX_MODULES];

void kernel_modules_init(void) {
    for (int i = 0; i < MAX_MODULES; i++) {
        loaded_modules[i].loaded = false;
    }
}

bool module_load(const char *name, uint64_t addr, uint64_t size) {
    for (int i = 0; i < MAX_MODULES; i++) {
        if (!loaded_modules[i].loaded) {
            loaded_modules[i].base_address = addr;
            loaded_modules[i].size = size;
            loaded_modules[i].loaded = true;
            int j = 0;
            while (name[j] != '\0' && j < 63) {
                loaded_modules[i].name[j] = name[j];
                j++;
            }
            loaded_modules[i].name[j] = '\0';
            return true;
        }
    }
    return false;
}

bool module_unload(const char *name) {
    (void)name;
    return true;
}
