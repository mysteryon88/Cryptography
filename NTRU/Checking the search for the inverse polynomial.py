from sympy import symbols, Poly, GF, rem
from sympy.polys.polytools import invert

x = symbols('x')

q = 11
N = 7

f = x**6-3*x**4+2*x**2+3

modulus = Poly(x**N - 1, x, domain=GF(q))

g = invert(Poly(f, x, domain=GF(q)), modulus)

check = rem(f * g, modulus)
check = Poly(check, x, domain=GF(q))

print("f(x):", f)
print("g(x):", g)
print("f(x)*g(x):", check)