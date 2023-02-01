#pragma once
#include <cstdint>
#include <vector>
#include "Point.h"

// y^2 = x^3 - a*x + b
// 4a^3 + 27b^2 != 0
class EllipticCurve
{
public: 
	double a;
	double b;
	int64_t mod;

	EllipticCurve(double _a, double _b, int64_t _mod);

	Point pointAdditionR(Point& P, Point& Q);
	Point scalarMultiplicationR(int64_t n, Point P);

	Point pointAdditionF(Point& P, Point& Q);
	Point scalarMultiplicationF(int64_t n, Point P);

private:
	std::vector <Point> points;

	void getCurvePoints();
	void negPointR(Point& P);
	void negPointF(Point& P);
	int64_t inverseOf(int64_t n);
	uint64_t getSubgroupOrder(Point P);
};

