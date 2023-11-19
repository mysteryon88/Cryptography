from sympy import symbols, Poly, GF

x = symbols('x')

q = 2
N = 4
a = 2 + 5*x**3
b = x + x**2 + 6*x**3

ab = a * b
ab = Poly(ab, x, domain=GF(q))

print("a(x)*b(x) =", ab)