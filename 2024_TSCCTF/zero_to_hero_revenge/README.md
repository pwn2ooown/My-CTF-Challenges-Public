# Zero to hero revenge

Now you're really cooking, hero. However it's 2024 now, can you still pwn this service?

I have patched the binary for you! Just make sure to both files are in the same directory as the executable, and you won't have libc version problem.

`nc host port`

Author: TWNWAKing

Hint 1(free): Do you know [how2heap](https://github.com/shellphish/how2heap)?

Hint 2(100 pts): You can search some keywords of this challenge on Google, and you will find some "really" useful information. What's the main difference of these two challenges? The libc version here is newer (2.35) so there must be some updates.(including security patches) How to bypass the "new checks" in new libc version? And [how2heap](https://github.com/shellphish/how2heap) is your friend. Maybe you need to read the source code when you're stuck. Try harder!
