# Babypwn2024 Nerf

It's 2024 now. Baby, pwn me first.

Notice that we enable POW (Proof-of-Work) to help our infra stay alive. See the attached files for more details.

Bruteforcing is not necessary and **prohibited**

`nc host port`

Author: TWNWAKing

Hint 1(free): There aren't many common and useful gadgets found by tools such as ROPgadget in this challenge. You may need to find useful gadgets for this challenge by yourself. `objdump` is your friend. Read the assembly!

Hint 2(100 pts): We know that the original overflow length is not enough so we need to somehow "overflow more". However after we can overflow a lot, we don't have any gadget to control rdi register in the challenge binary itself so we cannot perform classic exploit. We may still use gadgets in libc! So you may need to find a way to leak out an libc address first. There are many ways to do it. Maybe calling an "interesting" function in the binary itself and you can leak an libc address by combining it with some other magic gadgets you found. Try harder!
