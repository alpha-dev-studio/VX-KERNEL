import subprocess

p = subprocess.Popen([
    "qemu-system-x86_64",
    "-m", "2G",
    "-accel", "tcg",
    "-bios", "/usr/share/ovmf/OVMF.fd",
    "-drive", "file=build/vx-kernel.iso,format=raw,media=cdrom",
    "-boot", "d",
    "-serial", "file:serial.log",
    "-display", "none",
    "-no-reboot"
])

try:
    p.wait(timeout=5)
except subprocess.TimeoutExpired:
    p.kill()
    p.wait()

with open("serial.log", "rb") as f:
    print(f.read().decode(errors="ignore"))
