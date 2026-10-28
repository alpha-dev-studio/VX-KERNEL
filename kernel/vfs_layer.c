#include "vfs_layer.h"
#include <stddef.h>

static struct vfs_node *root_vfs = NULL;

void vfs_init(void) {
    root_vfs = NULL;
}

void vfs_mount(struct vfs_node *node) {
    node->next = root_vfs;
    root_vfs = node;
}

struct vfs_node *vfs_lookup(const char *name) {
    struct vfs_node *current = root_vfs;
    while (current != NULL) {
        int i = 0;
        bool match = true;
        while (current->name[i] != '\0' && name[i] != '\0') {
            if (current->name[i] != name[i]) {
                match = false;
                break;
            }
            i++;
        }
        if (match && current->name[i] == '\0' && name[i] == '\0') {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
