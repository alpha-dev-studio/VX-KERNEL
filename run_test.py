import subprocess
import sys

try:
    res = subprocess.run([
        "qemu-system-x86_64",
        "-m", "2G",
        "-accel", "tcg",
        "-bios", "/usr/share/ovmf/OVMF.fd",
        "-drive", "file=build/vx-kernel.iso,format=raw,media=cdrom",
        "-boot", "d",
        "-serial", "stdio",
        "-display", "none",
        "-no-reboot"
    ], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=5)
    output = res.stdout
except subprocess.TimeoutExpired as e:
    output = e.stdout if e.stdout else b""

print("--- QEMU OUTPUT ---")
print(output.decode(errors="ignore"))

if b"VX_KERNEL_READY" in output:
    print("\nSUCCESS: VX_KERNEL_READY detected!")
    sys.exit(0)
else:
    print("\nFAILURE: VX_KERNEL_READY not found.")
    sys.exit(1)
