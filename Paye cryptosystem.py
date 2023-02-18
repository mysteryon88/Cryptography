import math
import random

def L(u, n):
    return (u - 1)//(n)
    
def encryption(g, m, r, n):
    return pow(g,m,n*n) * pow(r, n, n*n) % (n*n)
    
def decryption(c, n, l, mu):
    return L(pow(c, l, n*n), n) * mu % n
    
def gcdExtended(a, b):
    if a == 0 :
        return b,0,1
    gcd,x1,y1 = gcdExtended(b%a, a)
    x = y1 - (b//a) * x1
    y = x1
    return gcd,x,y
    
def reciprocal(a, n):
    gcd, x, y = gcdExtended(a, n)
    if gcd == 1:
        return((x % n + n) % n)
    else:
        return(-1)
        
def homomorphism(a, b, n, g, l, mu):
    # choose rand r
    r = random.randint(0, n - 1)
    while (math.gcd(r, n) != 1):
        r = random.randint(0, n - 1)
    a1 = encryption(g, a, r, n)
    b1 = encryption(g, b, r, n)
    # multiplying encrypted numbers is equal to adding unencrypted numbers
    s1 = (a1 * b1) % (n*n)
    s = decryption(s1, n, l, mu)
    print(a, "+", b, "=", s)
    
def isPrime(x): 
    if (x == 2):
        return(1)
    i = 0
    while(i < 100):
        a = random.randint(0, x - 2) + 2
        if (math.gcd(a, x) != 1):
            return(0)
        if (pow(a, x - 1, x) != 1):
            return(0)
        i += 1
    return(1)
    
def main():
    while(True):
        # choose random options p and q
        p = random.randint(0, 2000000)
        while (isPrime(p) != 1):
            p = random.randint(0, 2000000)
        q = random.randint(0, 2000000)
        while (isPrime(q) != 1):
            q = random.randint(0, 2000000)
        n = p * q
        N = n * n
        f = (p - 1) * (q - 1)
        # !condition!
        if (math.gcd(n, f) == 1):
            break
    l = math.lcm(p - 1, q - 1)
    g = random.randint(0, N - 1)
    while (math.gcd(g, N) != 1):
        g = random.randint(0, N - 1)
    mu = reciprocal(L(pow(g, l, N), n), n) % n
    print("private key", n, g)
    print("public key", l, mu)
    # random open text
    m = random.randint(0, 2000000)
    print("open text", m)
    # choose rand r
    r = random.randint(0, n - 1)
    while (math.gcd(r, n) != 1):
        r = random.randint(0, n - 1)
    c = encryption(g, m, r, n)
    print("encrypted number", c)
    m1 = decryption(c, n, l, mu)
    if (m == m1):
        print("decrypted number", m1)
    # testing of homomorphic properties of a cryptosystem
    homomorphism(500, random.randint(10, 300), n, g, l, mu)
    
main()