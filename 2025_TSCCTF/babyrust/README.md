# Rusty Overflown Programming

- Pwn
- Medium

This time, R stands for [Rust](<https://en.wikipedia.org/wiki/Rust_(programming_language)>) in [ROP](https://en.wikipedia.org/wiki/Return-oriented_programming).

Author: pwn2ooown

## Solution

Just a simple ROP challenge but in rust. Not meant to be reverseable. One can use cyclic payload to get the padding length and the rest is just control arguments and call execve gadget. The gadgets should be enough.
