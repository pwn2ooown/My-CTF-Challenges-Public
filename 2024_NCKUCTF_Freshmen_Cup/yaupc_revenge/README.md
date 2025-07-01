# Yet Another Username and Password Checker Revenge

Yet Another Username and password checker but this time I enabled [some security features](https://en.wikipedia.org/wiki/Seccomp) to prevent some shellcode exploit. Can you still get the flag?

FYI, [here](https://github.com/pwn2ooown/my-ctf-challenges/tree/main/2023_freshmen/yaupc) is the original challenge from last year's NCKU Freshmen CTF.

Note: The flag is in the folder  `/home/yaupc-revenge`, and its name is `flag_[md5sum]`. For example, the filename would be like `flag_598f4d41b5657ea5b069403e450fe4ff`. So the full path of flag is `/home/yaupc-revenge/flag_598f4d41b5657ea5b069403e450fe4ff`.  **You need to find the correct file name of the flag in this challenge.**

Author: pwn2ooown

Only distribute binary.

## Solution

Since the first shellcode buffer is small we can generate a shellcode that reads a lot more to some memory, the rest is use `getdents` to find the flag filename and orw read it.
