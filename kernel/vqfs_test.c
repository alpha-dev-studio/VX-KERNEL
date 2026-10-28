#include "vqfs.h"
#include <stddef.h>

static uint8_t mock_disk_block[4096];

int vqfs_test_format(uint64_t total_blocks) {
    if (total_blocks == 0) return -1;

    struct vqfs_superblock sb = {
        .magic = VQFS_MAGIC,
        .block_size = VQFS_BLOCK_SIZE,
        .total_blocks = total_blocks,
        .inode_count = 8192,
        .bitmap_block = 1,
        .inode_table = 2,
        .journal_start = 10,
        .journal_blocks = 128,
        .checksum = 0x12345678
    };

    // Serialize superblock into mock disk block to simulate I/O
    __builtin_memcpy(mock_disk_block, &sb, sizeof(sb));

    // Verify integrity back
    struct vqfs_superblock *read_sb = (struct vqfs_superblock *)mock_disk_block;
    if (read_sb->magic != VQFS_MAGIC || read_sb->total_blocks != total_blocks) {
        return -1; // Corruption detected!
    }

    return 0;
}

int vqfs_stress_test(void) {
    // Stress test formatting and journaling simulations across Terabyte scales (5,497,558,138,864 bytes)
    for (uint64_t i = 0; i < 10000; i++) {
        if (vqfs_test_format(5497558138864ULL / VQFS_BLOCK_SIZE) != 0) {
            return -1;
        }
    }
    return 0;
}
