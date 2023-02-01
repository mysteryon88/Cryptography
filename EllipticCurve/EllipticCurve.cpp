#include "EllipticCurve.h"

EllipticCurve::EllipticCurve(double _a, double _b, int64_t _mod)
{
	a = _a;
	b = _b;
	mod = _mod;
	// getCurvePoints();
}

Point EllipticCurve::pointAdditionR(Point& P, Point& Q)
{
	Point R;

	if (R == Q) return P;

	double m = (P == Q) ?
		(3 * P.x * P.x + a) / 2 / P.y :
		(P.y - Q.y) / (P.x - Q.x);

	R.x = m * m - P.x - Q.x;
	R.y = -(P.y + m * (R.x - P.x));
	// R.y = -(Q.y + m * (R.x - Q.x));
	
	return R;
}

//double and add
Point EllipticCurve::scalarMultiplicationR(int64_t n, Point P)
{
	Point Q;
	int64_t result = 0;
	
	if (n != 0) 
	{
		if (n < 0)
		{
			n = -n;
			negPointR(P);
		}

		while (n)
		{
			if (n & 1)
				Q = pointAdditionR(P, Q);
			P = pointAdditionR(P, P);
			n >>= 1;
		}
		return Q;
	}

	Q.x = Q.y = NAN;
	return Q;
}

Point EllipticCurve::pointAdditionF(Point& P, Point& Q)
{
	Point R;
	int64_t m;

	//std::cout << P.x << " " << Q.x << std::endl;

	if (P.x != Q.x)
		m = (P.y - Q.y) * inverseOf(P.x - Q.x);
	else 
	{
		if (P.y == 0 && Q.y == 0) return R;
		else if (P.y == Q.y)
			m = (3 * P.x * P.x + a) * inverseOf(2 * P.y);
		else return R;
	}
	
	m %= mod;

	R.x = (int64_t)(m * m - P.x - Q.x) % mod;
	R.y = (int64_t)-(P.y + m * (R.x - P.x)) % mod;
	// R.y = (int64_t)-(Q.y + m * (R.x - Q.x)) % _mod;

	if (R.x < 0) R.x += mod;
	if (R.y < 0) R.y += mod;

	return R;
}

Point EllipticCurve::scalarMultiplicationF(int64_t n, Point P)
{
	Point Q = P;

	n %= getSubgroupOrder(P);
	if (n != 0)
	{
		for (auto i = 1; i < n; i++)
			Q = pointAdditionF(Q, P);
		return Q;
	}
	Q.x = Q.y = NAN;
	
	return Q;
}

void EllipticCurve::negPointR(Point& P)
{
	P.y = -P.y;
}

void EllipticCurve::negPointF(Point& P)
{
	P.y = mod - P.y;
}

int64_t EllipticCurve::inverseOf(int64_t n)
{

	n %= mod;

	if (n < 0) 
		n += mod;
	
	for (auto m = 0; m < mod; ++m) 
		if ((n * m) % mod == 1) return m;

	return NAN;
}

uint64_t EllipticCurve::getSubgroupOrder(Point P)
{
	uint64_t n = 2;
	Point Q = P;
	
	while (true) 
	{
		Q = pointAdditionF(Q, P);
		if (Q == Point(0, 0)) break;
		n++;
	}

	return n;
}

void EllipticCurve::getCurvePoints()
{
	for (auto x = 0; x < mod; ++x)
		for (auto y = 0; y < mod; ++y)
			if ((int64_t)(y * y - x * x * x - a * x - b) % mod == 0)
				points.push_back(Point(x, y));
}

