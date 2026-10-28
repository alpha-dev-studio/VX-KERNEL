CC = x86_64-linux-gnu-gcc
AS = x86_64-linux-gnu-as
LD = x86_64-linux-gnu-ld

CFLAGS = -Wall -Wextra -O2 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -mno-sse -mno-sse2 -mno-mmx -mno-avx -mcmodel=kernel -Ikernel
LDFLAGS = -nostdlib -T kernel/linker.ld -static

OBJS = build/kernel.o build/memory.o build/pmm.o build/vmm.o build/idt.o build/gdt.o build/syscall.o build/syscall_entry.o build/vqfs.o build/vqfs_test.o build/graphics.o build/input.o build/sched.o build/ipc.o build/drivers.o build/winapi.o build/object_manager.o build/net_stack.o build/vfs_layer.o build/security_acls.o build/kernel_modules.o
KERNEL_ELF = build/kernel.elf
ISO = build/vx-kernel.iso

.PHONY: all clean run iso

all: $(KERNEL_ELF)

build/kernel.o: kernel/kernel.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/kernel.c -o build/kernel.o

build/memory.o: kernel/memory.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/memory.c -o build/memory.o

build/pmm.o: kernel/pmm.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/pmm.c -o build/pmm.o

build/vmm.o: kernel/vmm.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/vmm.c -o build/vmm.o

build/idt.o: kernel/idt.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/idt.c -o build/idt.o

build/gdt.o: kernel/gdt.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/gdt.c -o build/gdt.o

build/syscall.o: kernel/syscall.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/syscall.c -o build/syscall.o

build/syscall_entry.o: kernel/syscall_entry.s
	@mkdir -p build
	$(AS) kernel/syscall_entry.s -o build/syscall_entry.o

build/vqfs.o: kernel/vqfs.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/vqfs.c -o build/vqfs.o

build/vqfs_test.o: kernel/vqfs_test.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/vqfs_test.c -o build/vqfs_test.o

build/graphics.o: kernel/graphics.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/graphics.c -o build/graphics.o

build/input.o: kernel/input.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/input.c -o build/input.o

build/sched.o: kernel/sched.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/sched.c -o build/sched.o

build/ipc.o: kernel/ipc.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/ipc.c -o build/ipc.o

build/drivers.o: kernel/drivers.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/drivers.c -o build/drivers.o

build/winapi.o: kernel/winapi.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/winapi.c -o build/winapi.o

build/object_manager.o: kernel/object_manager.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/object_manager.c -o build/object_manager.o

build/net_stack.o: kernel/net_stack.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/net_stack.c -o build/net_stack.o

build/vfs_layer.o: kernel/vfs_layer.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/vfs_layer.c -o build/vfs_layer.o

build/security_acls.o: kernel/security_acls.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/security_acls.c -o build/security_acls.o

build/kernel_modules.o: kernel/kernel_modules.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/kernel_modules.c -o build/kernel_modules.o

$(KERNEL_ELF): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(KERNEL_ELF)

# ---------------------------------------------------------------------------
# ISO targets
# ---------------------------------------------------------------------------
iso: $(KERNEL_ELF)
	@mkdir -p build/iso_root/boot/limine build/iso_root/EFI/BOOT
	@if [ -f "kernel/limine.h" ]; then \
		echo "Building Limine ISO..."; \
		cp $(KERNEL_ELF) build/iso_root/boot/kernel; \
		cp boot/limine.conf build/iso_root/boot/limine/limine.conf; \
		cp limine-bin/limine-bios-cd.bin build/iso_root/boot/limine/; \
		cp limine-bin/limine-bios.sys build/iso_root/boot/limine/; \
		cp limine-bin/limine-uefi-cd.bin build/iso_root/boot/limine/; \
		cp limine-bin/BOOTX64.EFI build/iso_root/EFI/BOOT/; \
		cp limine-bin/BOOTIA32.EFI build/iso_root/EFI/BOOT/; \
		xorriso -as mkisofs \
			-iso-level 3 \
			-R -J \
			-b boot/limine/limine-bios-cd.bin \
			-no-emul-boot -boot-load-size 4 -boot-info-table \
			--efi-boot boot/limine/limine-uefi-cd.bin \
			-efi-boot-part --efi-boot-image \
			--protective-msdos-label \
			build/iso_root -o $(ISO); \
		./limine-bin/limine bios-install $(ISO) || true; \
		echo "Limine ISO built successfully at $$(pwd)/$(ISO)"; \
	else \
		echo "Building bare ISO (kernel ELF only, not bootable)..."; \
		cp $(KERNEL_ELF) build/iso_root/boot/kernel; \
		xorriso -as mkisofs \
			-iso-level 3 \
			-R -J \
			build/iso_root -o $(ISO); \
		echo "Bare ISO built successfully at $$(pwd)/$(ISO)"; \
		echo "Note: Use ./add-limine.sh to make it bootable with Limine."; \
	fi

# ---------------------------------------------------------------------------
# Run targets
# ---------------------------------------------------------------------------
run: $(KERNEL_ELF)
	@if [ -f "kernel/limine.h" ]; then \
		if [ -f "$(ISO)" ]; then \
			qemu-system-x86_64 \
				-m 2G \
				-bios /usr/share/ovmf/OVMF.fd \
				-drive file=$(ISO),format=raw,media=cdrom \
				-boot d \
				-serial stdio \
				-no-reboot -no-shutdown; \
		else \
			echo "Limine ISO not found. Run: make iso"; \
			exit 1; \
		fi; \
	else \
		echo ""; \
		echo "VX-KERNEL is built as a bare kernel (no Limine bootloader)."; \
		echo "Run './add-limine.sh' to enable Limine boot."; \
		exit 1; \
	fi

clean:
	rm -rf build
