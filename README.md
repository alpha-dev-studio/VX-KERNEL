# VX-KERNEL

An original x86-64 hybrid operating system kernel designed from the ground up for high performance, modularity, and stability. 

## Default Build (No Bootloader)

By default, VX-KERNEL builds as a **bare, dependency-free kernel** with no bootloader attached. This makes it easy to embed, inspect, and modify without pulling in external bootloader code.

### Included Subsystems

- **Hybrid Architecture:** Combines microkernel-inspired service boundaries with monolithic performance.
- **Memory Management:** Physical Memory Manager (PMM) bitmap and Virtual Memory Manager (VMM) skeletons (require Limine boot info to initialize fully).
- **CPU & Interrupts:** Full Interrupt Descriptor Table (IDT) and CPU exception handling.
- **Privilege & System Calls:** GDT & Task State Segment (TSS) configured for Ring 0 / Ring 3 transitions with fast `syscall`/`sysret` MSR support.
- **VQFS Filesystem:** Custom high-performance filesystem designed with 4KB block alignments, inode indexing, journaling, and crash safety.
- **Embedded Hardware Drivers:** Built-in core drivers for AHCI/NVMe storage, Intel/AMD/NVIDIA graphics, networking, and USB HID.
- **Win64 API Subsystem:** Native Windows API compatibility layer designed to support Windows applications.
- **Kernel Objects:** Object manager, network stack, VFS layer, ACL security, dynamic kernel modules, and input handling.

## Enabling Limine Bootloader (Optional)

If you want to boot VX-KERNEL through Limine (UEFI/BIOS) with access to the bootloader-provided memory map, framebuffer, and Higher Half Direct Map (HHDM), run:

```bash
chmod +x add-limine.sh
./add-limine.sh
```

This script:
1. Adds `kernel/limine-protocol` as a git submodule.
2. Copies `kernel/limine-protocol/include/limine.h` to `kernel/limine.h`.
3. Patches `kernel/kernel.c` to request and consume Limine boot information.
4. Stages all changes for commit.

After running `add-limine.sh`:

```bash
git commit -m "Add Limine bootloader support"
make clean
make iso
python3 run_test.py   # or make run
```

To remove Limine again:

```bash
git submodule deinit -f kernel/limine-protocol
git rm -f kernel/limine-protocol kernel/limine.h
git checkout kernel/kernel.c  # or restore from kernel/kernel.c.no-limine.bak
```

## Building and Testing (Default Mode)

Requirements:
- WSL (Ubuntu) or Linux environment
- GCC cross-compiler (`x86_64-linux-gnu-gcc`), binutils, xorriso, and QEMU.

To build the kernel ELF:
```bash
make clean
make all
```

The default build produces `build/kernel.elf` — a freestanding ELF binary with no bootloader attached.

To build a raw ISO containing just the kernel ELF:
```bash
make iso
```

To run directly in QEMU **without Limine** (note: QEMU's `-kernel` requires a Linux-style boot header; use `add-limine.sh` for a proper bootable ISO):
```bash
make run
```

## Repository Layout

```
VX-KERNEL/
├── kernel/               # Kernel source (core + subsystems)
│   ├── kernel.c          # Kernel entry point (standalone by default)
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
├── boot/                 # Bootloader configuration (when Limine added)
├── add-limine.sh         # Script to re-add Limine bootloader support
├── Makefile              # Build system
├── README.md             # This file
├── LICENSE
└── .gitignore
```
