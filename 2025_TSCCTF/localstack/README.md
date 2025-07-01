# Localstack

- Pwn
- Easy

This is my stack implementation for data structure course's homework I. I've enabled all protections so it should be safe, right?

Author: pwn2ooown

## Solution

Use oob to overwrite the index of stack array and point to near return address to bypass canary. Then leak pie and overwrite the return address to win function.
