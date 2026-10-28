#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_KERNEL_OBJECTS 1024
#define OBJ_TYPE_FILE 1
#define OBJ_TYPE_THREAD 2
#define OBJ_TYPE_MUTEX 3
#define OBJ_TYPE_PROCESS 4

struct kernel_object {
    uint32_t id;
    uint32_t type;
    uint32_t permissions;
    char name[64];
    void *pointer;
    bool active;
};

void object_manager_init(void);
uint32_t obj_create(uint32_t type, const char *name, void *ptr, uint32_t perms);
struct kernel_object *obj_lookup(uint32_t id);
bool obj_close(uint32_t id);

#endif
