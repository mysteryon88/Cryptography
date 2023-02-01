#include <iostream>
#include "EllipticCurve.h"

int main()
{
	EllipticCurve C(2, 3, 67);
	Point P(1, 2.44949), Q(2.06934, 4), R;
	int64_t n = 4;

	R = C.pointAdditionR(P, Q);
	std::cout << "R = P + Q: " << R << std::endl;
	
	R = C.scalarMultiplicationR(n, P);
	std::cout << "R = n * P: " << R << std::endl;

	int64_t mod = 97; // prime!!!
	EllipticCurve C1(2, 3, mod);
	Point P1(17, 10), Q1(95, 31), R1;

	R1 = C1.pointAdditionF(P1, Q1);
	std::cout << "R = P + Q (mod " << mod << "): " << R1 << std::endl;

	R1 = C1.scalarMultiplicationF(n, P1);
	std::cout << "R = n * P (mod " << mod << "): " << R1 << std::endl;
	
	return 0;
}