#ifndef VFS_LAYER_H
#define VFS_LAYER_H

#include <stdint.h>
#include <stdbool.h>

struct vfs_node {
    char name[128];
    uint32_t flags;
    uint32_t length;
    uint32_t (*read)(struct vfs_node *node, uint32_t offset, uint32_t size, uint8_t *buffer);
    uint32_t (*write)(struct vfs_node *node, uint32_t offset, uint32_t size, uint8_t *buffer);
    struct vfs_node *next;
};

void vfs_init(void);
void vfs_mount(struct vfs_node *node);
struct vfs_node *vfs_lookup(const char *name);

#endif
