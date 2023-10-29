#include <iostream>
#include <vector>

#include "vecta.h"

typedef vecta::vec2d<double> Point;

// [ABP] = AB x AP
template <typename T>
T GetAreaFromPoints(vecta::vec2d<T> a, vecta::vec2d<T> b, vecta::vec2d<T> p)
{
	vecta::vec2d<T> AB = b - a;
	vecta::vec2d<T> AP = p - a;
	return AB ^ AP;
}

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

template <typename T>
bool IsBetween(T x, T a, T b)
{
	return a <= x && x <= b;
}


PointLocation GetPointLocation(const std::vector<Point>& polygon, Point pointToCheck)
{
	// The ray is horizontally placed, to fix y
	Point rayStart = pointToCheck;

	bool inside = false;
	for (int i = 0; i < polygon.size(); i++)
	{
		Point pointA = polygon[i];
		Point pointB = polygon[(i + 1) % polygon.size()];

		Point min = Point(std::min(pointA.x, pointB.x), std::min(pointA.y, pointB.y));
		Point max = Point(std::max(pointA.x, pointB.x), std::max(pointA.y, pointB.y));

		// check for horizontal lines
		if (pointA.y == pointB.y)
		{
			// it is horizontal
			if (pointB.y == rayStart.y && IsBetween(rayStart.x, min.x, max.x))
			{
				return PointLocation::Edge;
			}
		}
		else if (IsBetween(rayStart.y, min.y, max.y))
		{
			// Do not count if the ray intersects the vertex twice
			if (rayStart.y != max.y)
			{
				//if (rayStart.x > min.x)
				//{
				//	inside = !inside;
				//}
				double product = GetAreaFromPoints(pointA, pointB, pointToCheck);
				if (product > 0)
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