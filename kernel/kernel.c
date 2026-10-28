#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "limine.h"
#include "memory.h"
#include "pmm.h"
#include "vmm.h"
#include "idt.h"
#include "gdt.h"
#include "syscall.h"
#include "vqfs.h"
#include "vqfs_test.h"
#include "graphics.h"
#include "image.h"
#include "desktop.h"
#include "sched.h"
#include "ipc.h"
#include "drivers.h"
#include "winapi.h"
#include "object_manager.h"
#include "net_stack.h"
#include "vfs_layer.h"
#include "security_acls.h"
#include "kernel_modules.h"

// Request Limine base protocol revision 3
__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(3);

// Request UEFI / VBE graphical framebuffer from bootloader
__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

// Request physical memory map from bootloader
__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0
};

// Request Higher Half Direct Map (HHDM) offset for virtual memory mapping
__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST_ID,
    .revision = 0
};

// Limine request section markers
__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

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
    serial_print("\n[VeyroOS] Booting Veyro OS Desktop Environment...\n");

    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        serial_print("[VeyroOS] ERROR: Base revision not supported!\n");
        hcf();
    }

    if (memmap_request.response == NULL || hhdm_request.response == NULL) {
        serial_print("[VeyroOS] ERROR: Missing Limine memory map or HHDM response!\n");
        hcf();
    }

    pmm_init(memmap_request.response, hhdm_request.response->offset);
    vmm_init(hhdm_request.response->offset);
    idt_init();
    gdt_init();
    syscall_init();
    scheduler_init();

    object_manager_init();
    net_stack_init();
    vfs_init();
    kernel_modules_init();
    drivers_init_all();
    winapi_subsystem_init();
    desktop_init();

    if (framebuffer_request.response != NULL && framebuffer_request.response->framebuffer_count > 0) {
        struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];
        graphics_init(fb);

        // Render Veyro OS Glassmorphic Desktop & Prebuilt Apps Environment
        desktop_render_frame();
    }

    serial_print("VX_KERNEL_READY\n");
    hcf();
}
