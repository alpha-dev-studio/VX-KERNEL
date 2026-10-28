#include "object_manager.h"
#include <string.h>

static struct kernel_object object_pool[MAX_KERNEL_OBJECTS];
static uint32_t next_obj_id = 1;

void object_manager_init(void) {
    for (int i = 0; i < MAX_KERNEL_OBJECTS; i++) {
        object_pool[i].active = false;
    }
}

uint32_t obj_create(uint32_t type, const char *name, void *ptr, uint32_t perms) {
    for (int i = 0; i < MAX_KERNEL_OBJECTS; i++) {
        if (!object_pool[i].active) {
            object_pool[i].id = next_obj_id++;
            object_pool[i].type = type;
            object_pool[i].permissions = perms;
            object_pool[i].pointer = ptr;
            object_pool[i].active = true;

            // Simple string copy
            int j = 0;
            while (name[j] != '\0' && j < 63) {
                object_pool[i].name[j] = name[j];
                j++;
            }
            object_pool[i].name[j] = '\0';

            return object_pool[i].id;
        }
    }
    return 0; // Pool exhausted
}

struct kernel_object *obj_lookup(uint32_t id) {
    for (int i = 0; i < MAX_KERNEL_OBJECTS; i++) {
        if (object_pool[i].active && object_pool[i].id == id) {
            return &object_pool[i];
        }
    }
    return NULL;
}

bool obj_close(uint32_t id) {
    struct kernel_object *obj = obj_lookup(id);
    if (obj) {
        obj->active = false;
        return true;
    }
    return false;
}
