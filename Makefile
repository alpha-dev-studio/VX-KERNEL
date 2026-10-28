CC = x86_64-linux-gnu-gcc
LD = x86_64-linux-gnu-ld

CFLAGS = -Wall -Wextra -O2 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -mno-sse -mno-sse2 -mno-mmx -mno-avx -mcmodel=kernel -Ikernel
LDFLAGS = -nostdlib -T kernel/linker.ld -static

OBJS = build/kernel.o
KERNEL = build/kernel.elf
ISO = build/vx-kernel.iso

.PHONY: all clean run iso

all: $(ISO)

build/kernel.o: kernel/kernel.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c kernel/kernel.c -o build/kernel.o

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(KERNEL)

iso: $(ISO)

$(ISO): $(KERNEL)
	@echo "Building Limine ISO..."
	@mkdir -p build/iso_root/boot
	@mkdir -p build/iso_root/EFI/BOOT
	
	# Copy kernel ELF and config
	cp $(KERNEL) build/iso_root/kernel.elf
	cp boot/limine.conf build/iso_root/boot/limine.conf
	cp boot/limine.conf build/iso_root/boot/limine.cfg
	cp boot/limine.conf build/iso_root/limine.conf
	cp boot/limine.conf build/iso_root/limine.cfg
	
	# Copy Limine boot files
	cp limine-bin/limine-bios-cd.bin build/iso_root/
	cp limine-bin/limine-bios.sys build/iso_root/
	cp limine-bin/limine-uefi-cd.bin build/iso_root/
	cp limine-bin/BOOTX64.EFI build/iso_root/EFI/BOOT/
	
	# Create bootable ISO using xorriso with Rock Ridge (-R) and Joliet (-J)
	xorriso -as mkisofs \
		-iso-level 3 \
		-R -J \
		-b limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image \
		--protective-msdos-label \
		build/iso_root -o $(ISO)
	@echo "ISO built successfully at $(ISO)"

run: $(ISO)
	qemu-system-x86_64 \
		-m 2G \
		-bios /usr/share/ovmf/OVMF.fd \
		-drive file=$(ISO),format=raw,media=cdrom \
		-boot d \
		-serial stdio \
		-no-reboot -no-shutdown

clean:
	rm -rf build
