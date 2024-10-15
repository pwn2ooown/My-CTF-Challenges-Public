from Crypto.Cipher import AES
from Crypto.Hash import SHA256
from Crypto.Util.Padding import unpad
import sys


ENCRYPTED_FLAG = b'\x84:\xac\xf4\xcf~\xc9\x97\xf7.\xe81\xbc\xb4\xf4\\\xd0\x7f\xeb!\x98\xf2\xda)+\xcf\x86\x07|~\xc0\xb7\x88\xd1\xcb\xa2\xadu=\xdb\x04\xa3\x88\xa8\xfe\xea\xcd\x12!\xbd\xe3+?#\xad\xb8\xa6\x98\x9b\x87I\xe0\xc3\xa5\x8f#/\x93\xff\x1c\xeeGp\xf6j\x0c\xd5\xb0\xcb\x04\x99\x0c\xaf\xb6\xdf\xcf\xb2\x7f\xde\x93"9\x84\xce/\n7\xa7d*Y\x81.\x046r\xb9\xef\xbe\xf0\t\n\xe6\xa1\xf1\x16;;\xe6\xccC\n\xf3\xbc\'T\x01\xf4\xf9\xb8\xf6\xb2-P\xa5\x1a\xf5"\x19dT\x02\x1e\xfe&\xdf\xc3\x1e\xac\xde\xa5\xd8\xfe\xf0\xd0fW\x8c\x9e\xbb'

# Prompt
print('Welcome to the flag decryptor!')
print('Please make a file to store your all answers (in integers).')
print('Make sure there is not endline at the end.')

print('''The file is has to follow the format below:
<answer[0]>
<answer[1]>
<answer[2]>
...
<answer[N]>''')

print('''Here is an example that satisfies the format:
69
42
13
37
...
55''')
path = input('Please enter the file path of answer: ')
print(path)
with open(path, 'r') as fp:
    res = fp.read().strip().split('\n')
    print(res)
    answers = list(map(int, res))


# Key Derivation
sha256_obj = SHA256.new(data = bytes(answers))
key = sha256_obj.digest()
print(f'[*] Derived Key (May not able to decrypt): {sha256_obj.hexdigest()}')

# Flag decryption
try:
    aes_obj = AES.new(key=key, mode=AES.MODE_CBC, iv=bytes(answers[:16]))
    flag = unpad(aes_obj.decrypt(ENCRYPTED_FLAG), AES.block_size)
    print(f'[+] Here is your decrypted flag: {flag.decode()}')

except UnicodeDecodeError:
    print('[-] Error: Error decrypting the flag.')
    sys.exit(1)