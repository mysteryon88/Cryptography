#include "Point.h"

Point::Point()
{
	x = 0;
	y = 0;
}

Point::Point(double _x, double _y)
{
	x = _x;
	y = _y;
}

std::ostream& operator<<(std::ostream& os, const Point& point)
{
	return os << "x = " << point.x << " y = " << point.y << std::endl;
}

bool operator==(const Point& A, const Point& B)
{
	return A.x == B.x && A.y == B.y ? true : false;
}

bool operator!=(const Point& A, const Point& B)
{
	return A.x != B.x && A.y != B.y ? true : false;
}