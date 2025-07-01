# Babystack

By [Curious](https://github.com/Curious-Lucifer)

- Pwn
- Hard

## Solution

1. Overwrite `strlen` got entry of libc.got to control rip.
2. Control rip to `add rsp, 0x58` and rsp will be at you first input.
3. ROP-like and return to `one_gadget`.
