#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "limine.h"

// Base revision request (revision 6)
__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

// Framebuffer request
__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

// Markers
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
    serial_print("\n[VX-KERNEL] Booting VX-KERNEL (Hybrid Architecture, M0 Milestone)...\n");

    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        serial_print("[VX-KERNEL] ERROR: Base revision not supported!\n");
        hcf();
    }

    if (framebuffer_request.response != NULL && framebuffer_request.response->framebuffer_count > 0) {
        struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];
        volatile uint32_t *fb_ptr = fb->address;
        // Paint screen green (0x00FF00)
        for (uint64_t i = 0; i < (fb->width * fb->height); i++) {
            fb_ptr[i] = 0x0000FF00;
        }
    }

    serial_print("VX_KERNEL_READY\n");
    serial_print("[VX-KERNEL] Initialization complete. Entering controlled halt.\n");

    hcf();
}
