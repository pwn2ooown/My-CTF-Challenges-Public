# My CTF Challenges

CTF challenges I created and their writeups (most of them).

## 2023

### NCKUCTF Freshmen Cup (成大新生盃)

8 reverse/pwnable beginner friendly challenges.

## 2024

### AIS3 Pre-exam & MyFirstCTF

| Challenge     | Category | Keywords                                                                                         |
| ------------- | -------- | ------------------------------------------------------------------------------------------------ |
| base64encoder | Pwn      | array out-of-bound read, return to win                                                           |
| base64note    | Pwn      | CVE-2018-6789, use after free, heap off by one, code execution on glibc 2.35, libc GOT Hijacking |

### TSCCTF

| Challenge            | Category | Keywords                                                          |
| -------------------- | -------- | ----------------------------------------------------------------- |
| babypwn2024-nerf     | Pwn      | Stack pivoting, return to libc, one gadget                        |
| zero_to_hero_revenge | Pwn      | Glibc 2.35 tcache double free, safe linking bypass, GOT Hijacking |

### NCKUCTF Freshmen Cup (成大資安社期初新生盃 CTF)

| Challenge                                         | Category | Keywords                                                      |
| ------------------------------------------------- | -------- | ------------------------------------------------------------- |
| Baby Python Assembly                              | Rev      | Python Assembly                                               |
| babyheap                                          | Pwn      | Glibc 2.23 fastbin double free, malloc hook trick, one gadget |
| babyRSA                                           | Crypto   | RSA                                                           |
| E                                                 | Crypto   | RSA, Common Modulus Attack                                    |
| Every pwner's first challenge                     | Pwn      | Return to win, ret2win, ret2text, return to text              |
| Good hacker's website                             | Web      | PHP, Upload Bypass                                            |
| Good hacker's website Revenge                     | Web      | PHP, Upload Bypass                                            |
| Overflow Tutorial                                 | Pwn      | Integer overflow, buffer overflow                             |
| Yet Another Username and Password Checker Revenge | Pwn      | Return to shellcode, ret2sc, getdents, orw                    |
| We are good hackers                               | Misc     | LSB steganography, ASCII                                      |

### CGGC Qual

| Challenge                    | Category | Keywords                                                                                                                            |
| ---------------------------- | -------- | ----------------------------------------------------------------------------------------------------------------------------------- |
| Celerity Glide Google Chrome | Pwn      | Turboshaft type confusion, v8 pwn, browser exploitation, v8 sandox escape [330404819](https://issues.chromium.org/issues/330404819) |

### CGGC Final

| Challenge  | Category | Keywords                                                                                                                                                                            |
| ---------- | -------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Re:Sandbox | Pwn      | v8 sandbox escape, browser exploitation, [330404819](https://issues.chromium.org/issues/330404819) patch bypass via issue [344963941](https://issues.chromium.org/issues/344963941) |

## 2025

### TSCCTF

| Challenge   | Category | Keywords                                                     |
| ----------- | -------- | ------------------------------------------------------------ |
| localstack  | Pwn      | oob, ret2win, ret2text, bypass all protections, out of bound |
| globalstack | Pwn      | free hook, out of bound, oob                                 |
| babyrust    | Pwn      | rust pwn, ROP                                                |

### AIS3 Pre-exam & MyFirstCTF

| Challenge                          | Category         | Keywords                                                                       |
| ---------------------------------- | ---------------- | ------------------------------------------------------------------------------ |
| Welcome to the World of Ave Mujica | Pwn              | return to win                                                                  |
| AIS3 Tiny Web Server               | Misc / Rev / Pwn | Real world, 1 day, web server in C, ret2libc, backdoor(?), xor encryption, LFI |

## License

The content of this repository is licensed under the [CC-BY-4.0](https://choosealicense.com/licenses/cc-by-sa-4.0/).
