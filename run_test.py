import subprocess
import sys

p = subprocess.Popen([
    "qemu-system-x86_64",
    "-m", "2G",
    "-cdrom", "build/vx-kernel.iso",
    "-boot", "d",
    "-nographic",
    "-no-reboot"
], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

output = b""
try:
    for _ in range(100):
        line = p.stdout.readline()
        if not line:
            break
        output += line
        print(line.decode(errors="ignore"), end="")
        if b"VX_KERNEL_READY" in line:
            print("\nSUCCESS: VX_KERNEL_READY detected!")
            p.kill()
            sys.exit(0)
    p.kill()
except Exception as e:
    print(f"Error: {e}")
    p.kill()

print("\n--- QEMU OUTPUT ---")
print(output.decode(errors="ignore"))
sys.exit(1)
