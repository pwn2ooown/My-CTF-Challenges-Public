# GlobalStack

- Pwn
- Easy

Now I've learnt some global variables and pointer stuff, so I rewrote my stack implementation! It should be safe now, right?

Author: pwn2ooown

## Solution

Use oob to leak libc address and overwrite the pointer to gain arbitrary write. Overwrite `free_hook` to one gadget and shell out.
