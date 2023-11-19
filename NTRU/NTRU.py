import random
from sympy import symbols, Poly, GF, rem
from sympy.polys.polytools import invert

def generate_polynomial(n, num_pos_ones, num_neg_ones):
    poly_coeffs = [0] * n
    pos_indices = random.sample(range(n), num_pos_ones)
    neg_indices = random.sample(list(set(range(n)) - set(pos_indices)), num_neg_ones)

    for i in pos_indices:
        poly_coeffs[i] = 1
    for i in neg_indices:
        poly_coeffs[i] = -1

    poly = Poly(poly_coeffs, x)

    return poly

def generate_keypair(n, p, q, df, dg, dr):
    modulusP = Poly(x**n - 1, x, domain=GF(p))
    modulusQ = Poly(x**n - 1, x, domain=GF(q))
    
    # selection of a random polynomial f(x)
    f_gen = generate_polynomial(n, df, df-1)
    f = Poly(f_gen, x, domain=GF(q))

    # selection of a random polynomial f(x)
    g_gen = generate_polynomial(n, dg, dg)
    
    # search of inverse polynomials by moduli p and q
    # f(x)fp(x) = 1, f(x)fq(x) = 1
    fp = invert(Poly(f_gen, x, domain=GF(p)), modulusP)
    fq = invert(Poly(f_gen, x, domain=GF(q)), modulusQ)

    # public key calculation
    h = rem(fq * g_gen, x**n - 1)

    return f, fp, fq, g_gen, h

def encrypt_message(message, f, h, p, q, n, dr):

    # random polynomial selection
    r_gen = generate_polynomial(n, dr, dr)
    r = Poly(r_gen, x, domain=GF(p))

    e = rem(p * r * h + message, x**n - 1)
    e = Poly(e, x, domain=GF(q))

    return e

def decrypt_message(e, f, fp, p, q, n):
    a = rem(f * e, x**n - 1)
    a = Poly(a, x, domain=GF(q))

    # centering factors to [-(q - 1) / 2; (q - 1) / 2]
    coeffs = a.all_coeffs()
    half_q = (q - 1) // 2
    centered_coeffs = [coeff - q if coeff > q // 2 else coeff for coeff in coeffs]
    centered_a = Poly(centered_coeffs, x, domain=GF(p))

    m = rem(fp * centered_a, x**n - 1)
    m = Poly(m, x, domain=GF(p))

    return m

if __name__ == "__main__":
    x = symbols('x')
    n = 19
    # gcd(p, q) = 1
    p, q = 3, 31
    df, dg, dr = 6, 8, 5

    f, fp, fq, g_gen, h = generate_keypair(n, p, q, df, dg, dr)

    # message
    m = x**3 + x**2 + 1

    e = encrypt_message(m, f, h, p, q, n, dr)

    m1 = decrypt_message(e, f, fp, p, q, n)
    
    # converting polynomials to a single field for comparison in python
    m1 = Poly(m1, x, domain='ZZ')
    m = Poly(m, x, domain='ZZ')

    print("f(x):", f)
    print("fp(x):", fp)
    print("fq(x):", fq)
    print("g(x):", g_gen)
    print("h(x):", h, "\n")
    print("e(x):", e, "\n")
    print("m(x):", m)
    print("m1(x):", m1, "\n")
    print("Decryption result:", m == m1)