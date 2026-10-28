#include "syscall.h"
#include <stdint.h>

#define IA32_EFER 0xC0000080
#define IA32_STAR 0xC0000081
#define IA32_LSTAR 0xC0000082
#define IA32_FMASK 0xC0000084

static void wrmsr(uint32_t msr, uint64_t value) {
    uint32_t low = value & 0xFFFFFFFF;
    uint32_t high = value >> 32;
    __asm__ volatile("wrmsr" : : "c"(msr), "a"(low), "d"(high));
}

// Asm handler stub for syscall
extern void syscall_handler_entry(void);

void syscall_init(void) {
    // Enable SCE (System Call Extension) in EFER if needed, usually on by default in long mode

    // STAR MSR: bits 48-63 = User CS/SS, bits 32-47 = Kernel CS/SS
    // Kernel CS = 0x08, User CS (32-bit compat) = 0x1B (0x18 | 3), User CS (64-bit) = 0x20 | 3 = 0x23
    uint64_t star = ((uint64_t)0x0010 << 48) | ((uint64_t)0x08 << 32);
    wrmsr(IA32_STAR, star);

    // LSTAR MSR: target RIP for 64-bit syscall
    wrmsr(IA32_LSTAR, (uint64_t)syscall_handler_entry);

    // FMASK MSR: flags to clear on syscall
    wrmsr(IA32_FMASK, 0x200); // Clear interrupt flag
}
