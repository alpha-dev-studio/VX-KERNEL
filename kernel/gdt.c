#include "gdt.h"

struct gdt_descriptor {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

static struct gdt_entry gdt[7];
static struct tss_entry tss;
static struct gdt_descriptor gdt_desc;

void gdt_init(void) {
    gdt_desc.size = sizeof(gdt) - 1;
    gdt_desc.offset = (uint64_t)&gdt;

    // Null descriptor
    gdt[0] = (struct gdt_entry){0, 0, 0, 0, 0, 0};

    // Kernel Code Segment (Ring 0)
    gdt[1] = (struct gdt_entry){0, 0, 0, 0x9A, 0x20, 0}; 
    // Kernel Data Segment (Ring 0)
    gdt[2] = (struct gdt_entry){0, 0, 0, 0x92, 0x00, 0}; 
    // User Data Segment (Ring 3)
    gdt[3] = (struct gdt_entry){0, 0, 0, 0xF2, 0x00, 0}; 
    // User Code Segment (Ring 3)
    gdt[4] = (struct gdt_entry){0, 0, 0, 0xFA, 0x20, 0}; 

    // TSS descriptor (spans gdt[5] and gdt[6])
    uint64_t tss_base = (uint64_t)&tss;
    uint64_t tss_limit = sizeof(tss) - 1;

    gdt[5].limit_low = tss_limit & 0xFFFF;
    gdt[5].base_low = tss_base & 0xFFFF;
    gdt[5].base_middle = (tss_base >> 16) & 0xFF;
    gdt[5].access = 0x89; // Present, Ring 0, System, Available TSS
    gdt[5].granularity = (tss_limit >> 16) & 0x0F;
    gdt[5].base_high = (tss_base >> 24) & 0xFF;

    struct {
        uint32_t base_upper;
        uint32_t reserved;
    } *tss_upper = (void *)&gdt[6];
    tss_upper->base_upper = tss_base >> 32;
    tss_upper->reserved = 0;

    // Load GDT via inline assembly with explicit reload of segment registers and TR
    __asm__ volatile (
        "lgdt %0\n\t"
        "push $0x08\n\t"
        "lea 1f(%%rip), %%rax\n\t"
        "push %%rax\n\t"
        "lretq\n\t"
        "1:\n\t"
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        "mov $0x28, %%ax\n\t"
        "ltr %%ax\n\t"
        : : "m"(gdt_desc) : "rax", "memory", "cc"
    );
}
