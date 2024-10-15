# Overflow Tutorial

Do you know what is integer or buffer overflow? Come learn and play!

請直接連上機器就能看到題目了，在回答完問題後會給你 Flag。

Author: pwn2ooown

Note: No distribute binary

## Solution

5+251=256, however the size of number is 2 bytes we have an integer overflow so the result of 5+251 here is 0.

The buffer overflow part is calculating the offset of target and our input buffer and overflow the buffer.
