# Celerity Glide Google Chrome

Yet "really fast" another v8 challenge!

0-day exploits are welcome XD

Author: pwn2ooown

## Solution

You need to chain a v8 bug and a sandbox escape to get code execution.

1. Turboshaft type confusion, see [poc.js](poc.js)
2. Type Confusion to `addrof` / `fakeobj` primitive
3. `addrof` / `fakeobj` primitive to arbitrary r/w in V8 Sandbox
4. V8 Sandbox Escape 1 day <https://issues.chromium.org/issues/330404819>

This is a pretty old v8 version from March 2024 so 0 or 1 day exploits are welcome. Those are intended for this challenge.
