from sympy import symbols, Poly, rem

x = symbols('x')

N = 5
a = 2 + x + x**3
b = 8*x + 5*x**2 + 2*x**4

ab = a * b
ab = rem(a * b, x**N - 1)
ab = Poly(ab, x)
print("a(x)*b(x) mod(x^n - 1) =", ab)