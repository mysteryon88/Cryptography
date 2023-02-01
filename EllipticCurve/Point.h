#pragma once
#include <iostream> 
#include <ostream>

struct Point
{
	double x;
	double y;

	Point();
	Point(double _x, double _y);
};

std::ostream& operator<<(std::ostream& os, const Point& point);

bool operator==(const Point& A, const Point& B);

bool operator!=(const Point& A, const Point& B);