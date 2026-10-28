#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "memory.h"
#include "pmm.h"
#include "vmm.h"
#include "idt.h"
#include "gdt.h"
#include "syscall.h"
#include "vqfs.h"
#include "vqfs_test.h"
#include "graphics.h"
#include "input.h"
#include "sched.h"
#include "ipc.h"
#include "drivers.h"
#include "winapi.h"
#include "object_manager.h"
#include "net_stack.h"
#include "vfs_layer.h"
#include "security_acls.h"
#include "kernel_modules.h"

// Serial port definitions (COM1)
#define COM1_PORT 0x3F8

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void serial_init(void) {
    outb(COM1_PORT + 1, 0x00);
    outb(COM1_PORT + 3, 0x80);
    outb(COM1_PORT + 0, 0x03);
    outb(COM1_PORT + 1, 0x00);
    outb(COM1_PORT + 3, 0x03);
    outb(COM1_PORT + 2, 0xC7);
    outb(COM1_PORT + 4, 0x0B);
}

void serial_putchar(char a) {
    while ((inb(COM1_PORT + 5) & 0x20) == 0);
    outb(COM1_PORT, a);
}

void serial_print(const char* s) {
    while (*s) {
        serial_putchar(*s++);
    }
}

static void hcf(void) {
    __asm__ volatile ("cli");
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

void kmain(void) {
    serial_init();
    serial_print("\n[VX-KERNEL] Booting VX-KERNEL Core...\n");

    // Initialize core subsystems (standalone, no Limine boot info required)
    idt_init();
    gdt_init();
    syscall_init();
    scheduler_init();

    serial_print("[VX-KERNEL] Core initialization complete.\n");
    serial_print("Run ./add-limine.sh to enable full hardware support (PMM, VMM, drivers, framebuffer).\n");

    serial_print("VX_KERNEL_READY\n");
    hcf();
}
