# VX-KERNEL

An original x86-64 hybrid operating system kernel designed from the ground up for high performance, modularity, and stability. 

## Features

- **Hybrid Architecture:** Combines microkernel-inspired service boundaries with monolithic performance.
- **Bootloader:** Powered by the Limine boot protocol (supporting both UEFI and BIOS).
- **Memory Management:** Robust Physical Memory Manager (PMM) bitmap and Virtual Memory Manager (VMM) utilizing Limine's Higher-Half Direct Map (HHDM).
- **CPU & Interrupts:** Full Interrupt Descriptor Table (IDT) and CPU exception handling.
- **Privilege & System Calls:** GDT & Task State Segment (TSS) configured for Ring 0 / Ring 3 transitions with fast `syscall`/`sysret` MSR support.
- **VQFS Filesystem:** Custom high-performance filesystem designed with 4KB block alignments, inode indexing, journaling, and crash safety.
- **Embedded Hardware Drivers:** Built-in core drivers for AHCI/NVMe storage, Intel/AMD/NVIDIA graphics, networking, and USB HID.
- **Win64 API Subsystem:** Native Windows API compatibility layer designed to support Windows applications.

## Building and Testing

Requirements:
- WSL (Ubuntu) or Linux environment
- GCC cross-compiler (`x86_64-linux-gnu-gcc`), binutils, xorriso, and QEMU.

To build the bootable ISO and run automated tests:
```bash
make clean
make iso
python3 run_test.py
```

To run interactively in QEMU:
```bash
make run
```
