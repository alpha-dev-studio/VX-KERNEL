#include "idt.h"

static struct idt_entry idt[256];
static struct idt_ptr idtp;

extern void idt_flush(uint64_t);

// Basic exception handler
void exception_handler(void) {
    __asm__ volatile ("cli; hlt");
}

void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = base & 0xFFFF;
    idt[num].selector = sel;
    idt[num].ist = 0;
    idt[num].flags = flags;
    idt[num].offset_mid = (base >> 16) & 0xFFFF;
    idt[num].offset_high = (base >> 32) & 0xFFFFFFFF;
    idt[num].zero = 0;
}

void idt_init(void) {
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint64_t)&idt;

    // Load IDT pointer
    __asm__ volatile ("lidt %0" : : "m"(idtp));
}
