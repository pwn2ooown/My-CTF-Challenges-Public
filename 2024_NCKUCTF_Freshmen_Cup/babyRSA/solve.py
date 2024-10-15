from Crypto.PublicKey import RSA
from Crypto.Util.number import long_to_bytes, bytes_to_long
import rsa
# Read the private key from a file
with open('aaa.pem', 'r') as key_file:
    private_key = RSA.import_key(key_file.read())

# Example raw RSA encrypted data (ensure this matches the key size in bytes)
with open('encrypted.bin', 'rb') as encrypted_file:
    encrypted_data = encrypted_file.read()
print(hex(private_key.d))
print(long_to_bytes(pow(bytes_to_long(encrypted_data), private_key.d, private_key.n)))

