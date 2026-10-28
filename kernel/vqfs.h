#ifndef VQFS_H
#define VQFS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define VQFS_MAGIC 0x56514653 // "VQFS"
#define VQFS_BLOCK_SIZE 4096
#define VQFS_MAX_FILENAME 256

// Superblock: resides at block 0
struct vqfs_superblock {
    uint32_t magic;          // Must be VQFS_MAGIC
    uint32_t block_size;     // 4096
    uint64_t total_blocks;   // Total blocks on device/partition
    uint64_t inode_count;    // Total inodes available
    uint64_t bitmap_block;   // Block index of block allocation bitmap
    uint64_t inode_table;    // Block index of inode table
    uint32_t journal_start;  // Starting block of journal log
    uint32_t journal_blocks; // Number of blocks reserved for journaling
    uint32_t checksum;       // CRC32 or simple hash of superblock
} __attribute__((packed));

// Inode structure (e.g. 128 bytes each)
struct vqfs_inode {
    uint32_t mode;           // File type and permissions
    uint32_t uid;            // Owner User ID
    uint32_t gid;            // Owner Group ID
    uint64_t size;           // File size in bytes
    uint64_t ctime;          // Creation timestamp
    uint64_t mtime;          // Modification timestamp
    uint32_t direct_blocks[12]; // Direct block pointers
    uint32_t indirect_block; // Single indirect block pointer
    uint32_t flags;          // Corruption prevention / flags
    uint32_t checksum;       // Inode integrity checksum
} __attribute__((packed));

// Directory entry structure
struct vqfs_dirent {
    uint32_t inode;          // Inode number
    uint8_t file_type;       // File or directory
    uint8_t name_len;        // Length of filename
    char name[VQFS_MAX_FILENAME]; // Null-terminated filename
} __attribute__((packed));

// Journal transaction header for crash prevention
struct vqfs_j_header {
    uint32_t magic;          // Transaction magic
    uint32_t tx_id;          // Transaction ID
    uint32_t block_count;    // Number of blocks in this transaction
    uint32_t checksum;       // Checksum of transaction data
} __attribute__((packed));

void vqfs_spec_init(void);

#endif
