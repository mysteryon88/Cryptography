import hashlib
import random
import gmpy2
from gmpy2 import mpz

# (a * x) % b == 1
def extended_euclidean(a, b):
    _, x, _ = gmpy2.gcdext(a, b)
    return x % b

def find_large_prime(bits):
    while True:
        # Generate a random number within the bit range
        n = mpz(random.getrandbits(bits))
        # Ensure the number is odd
        n |= 1
        # Check if the number is prime
        if gmpy2.is_prime(n):
            return n

def calculate_hash(*args):
    concatenated = ''.join(map(str, args))
    return int.from_bytes(hashlib.sha256(concatenated.encode('utf-8')).digest(), 'little', signed=True)

g = 5
h = 3
p = pow(2, 255)
# secret
x = find_large_prime(16)

Y = pow(g, x, p)
Z = pow(h, x, p)

k = find_large_prime(16)

# commitments
A1 = pow(g, k, p)
B1 = pow(h, k, p)

# random oracle (non-interactive proofs)
c1 = str(Y) + str(Z) + str(A1) + str(B1)
hash_1 = hashlib.sha256(c1.encode('utf-8'))  
c1 = int.from_bytes(hash_1.digest(), 'little', signed='True')

s = (k - c1 * x) % p

# proof: (c1,s)

if s < 0:
    val1 = gmpy2.powmod(g, -s, p)
    val2 = gmpy2.powmod(h, -s, p)
    val1 = extended_euclidean(val1, p)
    val2 = extended_euclidean(val2, p)
else:
    val1 = gmpy2.powmod(g, s, p)
    val2 = gmpy2.powmod(h, s, p)

if c1 < 0:
    val3 = gmpy2.powmod(Y, -c1, p)
    val4 = gmpy2.powmod(Z, -c1, p)
    val3 = extended_euclidean(val3, p)
    val4 = extended_euclidean(val4, p)
else:
    val3 = gmpy2.powmod(Y, c1, p)
    val4 = gmpy2.powmod(Z, c1, p)

A2 = (val1 * val3) % p
B2 = (val2 * val4) % p

c2 = str(Y) + str(Z) + str(A2) + str(B2)
hash_2 = hashlib.sha256(c2.encode('utf-8'))  
c2 = int.from_bytes(hash_2.digest(), 'little', signed='True')

if c1 == c2:
    print("Proved!")
else:
    print("Not Proved!")

print(c1)
print(c2)