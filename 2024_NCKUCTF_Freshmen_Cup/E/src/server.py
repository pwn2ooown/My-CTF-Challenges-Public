from Crypto.Util.number import *
from secret import flag

n = 1
for _ in range(2):
    n *= getPrime(1024)

m = bytes_to_long(flag)

try:
    print(f"Hint: {n=}")
    for i in range(2):
        print(f'You have {2-i} chance(s).')
        e = int(input(f'Give me your e> '))
        if (e.bit_length() < 20) or (e < 0):
            print(f'Your e is too small!')
            exit()
        print(f'{pow(m, e, n)}')
except:
    print(f'Something is not good!')
    exit()
