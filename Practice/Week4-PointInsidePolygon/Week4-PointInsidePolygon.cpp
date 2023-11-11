#include <iostream>
#include <vector>

#include "vecta.h"

typedef vecta::vec2d<double> Point;

enum class Orientation
{
	Colinear,
	Clockwise,
	CounterClockWise,
};

enum class PointLocation
{
	Inside,
	Outside,
	Edge,
};

const char* ToString(PointLocation location)
{
	switch (location)
	{
	case PointLocation::Inside: return "Inside";
	case PointLocation::Outside: return "Outside";
	case PointLocation::Edge: return "Edge";
	default: return "";
	}
}

// [ABP] = AB x AP
double GetAreaFromPoints(Point a, Point b, Point p)
{
	Point AB = b - a;
	Point AP = p - a;
	return AB ^ AP;
}

Orientation GetOrientation(Point a, Point b, Point p)
{
	double area = GetAreaFromPoints(a, b, p);
	if (area == 0.0)
	{
		return Orientation::Colinear;
	}
	return area < 0.0 ? Orientation::Clockwise : Orientation::CounterClockWise;
}

Orientation GetPolygonOrientation(const std::vector<Point>& polygon)
{
	double area = 0.0;
	for (int i = 0; i < polygon.size(); i++)
	{
		Point L = polygon[i ? i - 1 : polygon.size() - 1];
		Point B = polygon[i];
		Point R = polygon[(i + 1) % polygon.size()];
		area += GetAreaFromPoints(L, B, R);
	}
	if (area == 0.0)
	{
		return Orientation::Colinear;
	}
	return area < 0.0 ? Orientation::Clockwise : Orientation::CounterClockWise;
}

template <typename T>
bool IsBetween(T x, T a, T b)
{
	return a <= x && x <= b;
}


PointLocation GetPointLocation(const std::vector<Point>& polygon, Point P)
{
	Orientation polygonOrientation = GetPolygonOrientation(polygon);
	// The ray is horizontally placed, to fix y
	bool inside = true;
	for (int i = 0; i < polygon.size(); i++)
	{
		Point A = polygon[i];
		Point B = polygon[(i + 1) % polygon.size()];

		Point min = Point(std::min(A.x, B.x), std::min(A.y, B.y));
		Point max = Point(std::max(A.x, B.x), std::max(A.y, B.y));

		// check for horizontal lines
		if (A.y == B.y)
		{
			// it is horizontal
			if (B.y == P.y && IsBetween(P.x, min.x, max.x))
			{
				return PointLocation::Edge;
			}
		}
		else
		{
			// Do not count if the ray intersects the vertex twice
			if (P.y < max.y && P.y >= min.y)
			{
				Orientation orientation = GetOrientation(A, B, P);
				if (orientation == polygonOrientation)
				{
					inside = !inside;
				}
			}
			
		}
	}

	return inside ? PointLocation::Inside : PointLocation::Outside;
}


int main()
{
	// TestCase Edge:       4 0 0 10 0 10 10 0 10 0 0  
	// TestCase Inside:     4 0 0 10 0 10 10 0 10 2 1  
	// TestCase Outside:    4 0 0 10 0 10 10 0 10 -2 1 
	int n;
	std::cin >> n;

	std::vector<Point> polygon(n);
	for (Point& p : polygon)
	{
		std::cin >> p;
	}

	Point pointToCheck;
	std::cin >> pointToCheck;

	PointLocation location = GetPointLocation(polygon, pointToCheck);
	std::cout << ToString(location);
}