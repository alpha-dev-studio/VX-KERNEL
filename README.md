# VX-KERNEL

An original x86-64 hybrid operating system kernel designed from the ground up for high performance, modularity, and stability.

## Included Subsystems

- **Hybrid Architecture:** Combines microkernel-inspired service boundaries with monolithic performance.
- **Memory Management:** Physical Memory Manager (PMM) bitmap and Virtual Memory Manager (VMM).
- **CPU & Interrupts:** Full Interrupt Descriptor Table (IDT) and CPU exception handling.
- **Privilege & System Calls:** GDT & Task State Segment (TSS) configured for Ring 0 / Ring 3 transitions with fast `syscall`/`sysret` MSR support.
- **VQFS Filesystem:** Custom high-performance filesystem designed with 4KB block alignments, inode indexing, journaling, and crash safety.
- **Embedded Hardware Drivers:** Built-in core drivers for AHCI/NVMe storage, Intel/AMD/NVIDIA graphics, networking, and USB HID.
- **Win64 API Subsystem:** Native Windows API compatibility layer designed to support Windows applications.
- **Kernel Objects:** Object manager, network stack, VFS layer, ACL security, dynamic kernel modules, and input handling.

## Building and Testing

Requirements:
- WSL (Ubuntu) or Linux environment
- GCC cross-compiler (`x86_64-linux-gnu-gcc`), binutils, and QEMU.

To build the kernel ELF:

```bash
make clean
make all
```

This produces `build/kernel.elf` — a standalone, freestanding x86-64 kernel with no external bootloader dependencies.

To inspect the built kernel:

```bash
file build/kernel.elf
```

## Repository Layout

```
VX-KERNEL/
├── kernel/               # Kernel source (core + subsystems)
│   ├── kernel.c          # Kernel entry point
│   ├── memory.c/h        # Physical & virtual memory management
│   ├── pmm.c/h           # Physical memory manager (bitmap)
│   ├── vmm.c/h           # Virtual memory manager (page tables)
│   ├── idt.c/h           # Interrupt Descriptor Table
│   ├── gdt.c/h           # Global Descriptor Table & TSS
│   ├── syscall.c/h       # System call entry (MSR/SYSCALL)
│   ├── sched.c/h         # Process scheduler
│   ├── ipc.c/h           # Inter-process communication
│   ├── drivers.c/h       # Embedded hardware drivers
│   ├── winapi.c/h        # Windows API compatibility shims
│   ├── object_manager.c/h# Kernel object manager
│   ├── net_stack.c/h     # Network stack skeleton
│   ├── vfs_layer.c/h     # Virtual filesystem layer
│   ├── security_acls.c/h # Access control lists
│   ├── kernel_modules.c/h# Dynamic kernel modules
│   ├── vqfs.c/h          # VQFS filesystem
│   ├── vqfs_test.c/h     # VQFS stress tests
│   ├── graphics.c/h      # Basic framebuffer graphics
│   ├── input.c/h         # Keyboard & mouse input
│   ├── boot.s            # Assembly boot stub
│   └── linker.ld         # Linker script
├── Makefile              # Build system
├── README.md             # This file
├── LICENSE
└── .gitignore
```
