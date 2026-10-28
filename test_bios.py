import subprocess

p = subprocess.Popen([
    "qemu-system-x86_64",
    "-m", "2G",
    "-cdrom", "build/veyro-os.iso",
    "-boot", "d",
    "-nographic",
    "-no-reboot"
], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

try:
    out, _ = p.communicate(timeout=4)
    print(out.decode(errors="ignore"))
except subprocess.TimeoutExpired:
    p.kill()
    out, _ = p.communicate()
    print(out.decode(errors="ignore"))
