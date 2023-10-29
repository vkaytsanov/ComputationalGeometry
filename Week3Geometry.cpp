#include <iostream>
#include <vector>

#include "vecta.h"

typedef vecta::vec2d<double> Point;


enum class PointLocation
{
    Inside,
    Outside,
    Edge,
};


PointLocation GetPointLocation(const std::vector<Point>& polygon, Point pointToCheck)
{
    // The ray is horizontally placed, to fix y
    Point rayStart = pointToCheck;

    int intersections = 0;
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
            if (min.x >= rayStart.x && rayStart.x <= max.x && pointB.y == rayStart.y)
            {
                return PointLocation::Edge;
            }
        }

        // Do not count if the ray intersects the vertex twice
        if (max.y != rayStart.y)
        {
            // licevo proizvedenie [12P]
            Point P12 = pointB - pointA;
            Point P1P = pointToCheck - pointA;
            auto product = P12 ^ P1P;
            if (product > 0)
            {
                intersections++;
            }
        }
    }

    return intersections % 2 == 0 ? PointLocation::Outside : PointLocation::Inside;
}


void CheckIfPointIsInside()
{
    int n;
    std::cin >> n;

    std::vector<Point> points;
    for (int i = 0; i < n; i++)
    {
        Point point;
        std::cin >> point;
        points.push_back(point);
    }

    Point pointToCheck;
    std::cin >> pointToCheck;

    PointLocation location = GetPointLocation(points, pointToCheck);
    std::cout << int(location);
}

int main()
{
    // TestCase Edge:       4 0 0 3 0 3 3 0 3 0 0   Correct
    // TestCase Outside:    4 0 0 3 0 3 3 0 3 -2 1  Incorrect
    // TestCase Inside:     4 0 0 3 0 3 3 0 3 2 1  Incorrect
    CheckIfPointIsInside();


}