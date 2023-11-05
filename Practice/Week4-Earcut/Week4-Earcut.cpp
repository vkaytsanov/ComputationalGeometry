#include <iostream>
#include <vector>

#include "vecta.h"

typedef vecta::vec2d<double> Point;

struct Triangle
{
	Point A;
	Point B;
	Point C;
};

template <typename T>
T GetAreaFromPoints(vecta::vec2d<T> a, vecta::vec2d<T> b, vecta::vec2d<T> p)
{
	vecta::vec2d<T> AB = b - a;
	vecta::vec2d<T> AP = p - a;
	return AB ^ AP;
}

std::vector<Triangle> Earcut(std::vector<Point> polygon)
{
	int orientation = 0;
	for (int i = 0; i < polygon.size(); i++)
	{
		Point a = polygon[i];
		Point b = polygon[(i + 1) % polygon.size()];
		orientation += (a.x * b.y) - (a.y * b.x);
	}

	//std::cout << orientation;

	std::vector<Triangle> triangles;
	int i = 1;
	while (polygon.size() > 3)
	{
		Point A = polygon[i ? i - 1 : polygon.size() - 1];
		Point B = polygon[i];
		Point C = polygon[(i + 1) % polygon.size()];
		bool isEar = GetAreaFromPoints(A, B, C) * orientation > 0;

		if (isEar)
		{
			triangles.push_back({ A, B, C });
			polygon.erase(polygon.begin() + i);
			i = 0;
		}
		else
		{
			i++;
		}
	}
	triangles.push_back({ polygon[2], polygon[0], polygon[1] });
	return triangles;
}

int main()
{
	// Test Case 1: 4 0 0 10 0 10 10 0 10
	// Test Case 2: 5 0 0 10 0 10 10 15 15 0 15
	int n;
	std::cin >> n;

	std::vector<Point> polygon(n);
	for (Point& p : polygon)
	{
		std::cin >> p;
	}

	std::vector<Triangle> triangles = Earcut(polygon);
	for (int i = 0; i < triangles.size(); i++)
	{
		const Triangle& tri = triangles[i];
		Point A = tri.A;
		Point B = tri.B;
		Point C = tri.C;
		printf("Triangle %d: A(%.2f, %.2f) B(%.2f, %.2f) C(%.2f, %.2f)\n", i, A.x, A.y, B.x, B.y, C.x, C.y);
	}

	// SP
	printf("\n");
	for (int i = 0; i < triangles.size(); i++)
	{
		const Triangle& tri = triangles[i];
		Point A = tri.A;
		Point B = tri.B;
		Point C = tri.C;
		printf("%.1f %.1f %.1f %.1f\n", A.x, A.y, B.x, B.y);
		printf("%.1f %.1f %.1f %.1f\n", B.x, B.y, C.x, C.y);
		printf("%.1f %.1f %.1f %.1f\n", C.x, C.y, A.x, A.y);
	}
}
