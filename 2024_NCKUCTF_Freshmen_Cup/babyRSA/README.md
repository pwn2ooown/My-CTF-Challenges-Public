# BabyRSA

Given an RSA private key and an encrypted file, how to decrypt RSA?

Author: pwn2ooown

## Solution

many ways to solve

Solve 1: ssh-keygen -p -f id_rsa -m pem convert to PEM format and use cyberchef
Solve 2: use solve.py
solve 3: `openssl rsa -in aaa.pem -text -inform PEM -noout` to see the private exponent and N, then parse it out by yourself and decrypt RSA.
