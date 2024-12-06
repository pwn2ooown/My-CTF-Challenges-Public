# Re:從零開始的沙箱逃逸生活

V8 sandbox escape from qualification challenge is just too easy! Can you escape out of the V8 sandbox again?

0-day exploits are always appreciated XD

Author: pwn2ooown

## Solution


0. The sandbox escape from the qualification challenge is not working in this version since the root cause of the vulnerability is some stack out-of-bound to ROP. (<https://github.com/rycbar77/V8-Sandbox-Escape-via-Regexp>) V8 team has added a "bound check" to prevent this kind of attack. (<https://source.chromium.org/chromium/chromium/src/+/main:v8/src/regexp/regexp-interpreter.cc;l=201>)
1. The vulnerability <https://issues.chromium.org/issues/344963941> here is another stack out-of-bound write caused by `backtrack_stack `. Modify the public POC to over write `total_register_count_` to bypass the patch.
2. The rest is same as stack out-of-bound to ROP.
