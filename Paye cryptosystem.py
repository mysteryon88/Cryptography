import math
import random

def L(u, n):
    return (u - 1)//(n)
    
def encryption(g, m, r, n):
    return pow(g, m, n*n) * pow(r, n, n*n) % (n*n)
    
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
        
def homomorphismSum(a, b):
    n, g, l, mu = generateKey()
    # choose rand r
    ra = random.randint(0, n - 1)
    rb = random.randint(0, n - 1)
    
    print("ra =", ra)
    print("rb =", rb)
    a1 = encryption(g, a, ra, n)
    b1 = encryption(g, b, rb, n)
    
    # multiplying encrypted numbers is equal to adding unencrypted numbers
    s1 = (a1 * b1) % (n*n)
    s = decryption(s1, n, l, mu)
    if (s == (a + b)):
        print(a, "+", b, "=", s, "\n")
    else:
        print("ERROR\n")
        
def homomorphismDiff(a, b):
    n, g, l, mu = generateKey()
    # choose rand r
    ra = random.randint(0, n - 1)
    rb = random.randint(0, n - 1)
    b1 = n - b
    print("ra =", ra)
    print("rb =", rb)
    a1 = encryption(g, a, ra, n)
    b1 = encryption(g, b1, rb, n)
    
    # multiplying encrypted numbers is equal to adding unencrypted numbers
    s1 = (a1 * b1) % (n*n)
    s = decryption(s1, n, l, mu)
    if (s == (a - b)):
        print(a, "-", b, "=", s)
    else:
        print("ERROR\n")
    
def isPrime(x): 
    if (x == 2):
        return(True)
    i = 0
    while(i < 100):
        a = random.randint(0, x)
        if (math.gcd(a, x) != 1):
            return(False)
        if (pow(a, x - 1, x) != 1):
            return(False)
        i += 1
    return(True)
    
def generateKey():
    while(True):
        # choose random options p and q
        p = random.randint(0, 20000)
        while (isPrime(p) != True):
            p = random.randint(0, 20000)
        q = random.randint(0, 20000)
        while (isPrime(q) != 1):
            q = random.randint(0, 20000)
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
    return n, g, l, mu
    
def main():
    n, g, l, mu = generateKey()
    # random open text
    m = random.randint(0, 1000)
    print("open text", m)
    # choose rand r
    #r = random.randint(0, n - 1)
    #while (math.gcd(r, n) != 1):
    r = random.randint(0, n - 1)
    print("r =", r)
    c = encryption(g, m, r, n)
    print("encrypted number", c)
    m1 = decryption(c, n, l, mu)
    if (m == m1):
        print("decrypted number", m1, "\n")
    else:
        print("ERROR \n")
        
    # testing of homomorphic properties of a cryptosystem
    homomorphismSum(500, random.randint(10, 300))
    homomorphismDiff(500, random.randint(10, 300))
    
main()