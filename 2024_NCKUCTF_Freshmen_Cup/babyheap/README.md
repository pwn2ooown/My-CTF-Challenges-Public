# Babyheap

Baby heap challenge! XD

I've patched the binary for you! Just make sure to both files are in the same directory as the executable, and you won't have libc and loader version problem.

![](meme.png)

Author: pwn2ooown

## Solution

I don't own this challenge, I forgot where this challenge comes from. But the solution is quite simple: use UAF to leak libc address fastbin double free to allocate a chunk at `__malloc_hook` and overwrite it with `one_gadget`.

You need to use "double free trick" to trigger malloc or use realloc_hook to adjust stack to satisfy the constraints of one_gadget.
