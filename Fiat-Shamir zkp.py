from math import gcd
from random import randint

class FiatShamirProtocol:
    def __init__(self, p, q, s):
        self.n = p * q
        self.s = s
        assert gcd(self.s, self.n) == 1, "s and n must be coprime."
        self.v = pow(self.s, 2, self.n)

    def step_1(self, x):
        """Step 1: A selects a random number x and computes y"""
        self.y = pow(x, 2, self.n)
        return self.y

    def step_2(self):
        """Step 2: B sends a random challenge bit b to A"""
        self.b = randint(0, 1)
        return self.b

    def step_3(self, x):
        """Step 3: A computes the response y' based on b"""
        self.y_prime = (x * pow(self.s, self.b, self.n)) % self.n
        return self.y_prime

    def step_4(self):
        """Step 4: B verifies A's response"""
        if pow(self.y_prime, 2, self.n) == (self.y * pow(self.v, self.b, self.n)) % self.n:
            return True
        else:
            return False

# Parameters
p = 683
q = 811
s = 43215 # For A

# Initialize the protocol
fsp = FiatShamirProtocol(p, q, s)

# Perform the protocol 20 times
verification_results = []
for _ in range(20):
    x = randint(2, fsp.n - 1) # A selects a random x
    y = fsp.step_1(x) # A computes y
    b = fsp.step_2() # B sends a random bit b
    y_prime = fsp.step_3(x) # A computes y'
    verification_results.append(fsp.step_4()) # B verifies the response

print(verification_results)
