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

PointLocation GetPointLocationFromAngle(Point A, Point B, Point C, Point P)
{
    Orientation BCP = GetOrientation(B, C, P);
    Orientation ABP = GetOrientation(A, B, P);

    if (BCP == Orientation::Colinear || ABP == Orientation::Colinear)
    {
        Point AP = P - A;
        Point BP = P - B;
        Point CP = P - C;
        double BPDotCP = BP * CP;
        double APDotBP = AP * BP;
        if ((BCP == Orientation::Colinear && BPDotCP < 0) || (ABP == Orientation::Colinear && APDotBP < 0) || (APDotBP == 0.0 && BPDotCP == 0.0))
        {
            return PointLocation::Edge;
        }
    }
    else if (BCP == ABP)
    {
        return PointLocation::Inside;
    }

    return PointLocation::Outside;
}

PointLocation GetPointLocationLinear(const std::vector<Point>& polygon, Point P)
{
    Orientation polygonOrientation = GetPolygonOrientation(polygon);

    for (int i = 0; i < polygon.size(); i++)
    {
        Point L = polygon[i ? i - 1 : polygon.size() - 1];
        Point B = polygon[i];

        Orientation orientation = GetOrientation(L, B, P);
        if (orientation == Orientation::Colinear)
        {
            return PointLocation::Edge;
        }

        if (polygonOrientation != orientation)
        {
            return PointLocation::Outside;
        }
    }

    return PointLocation::Inside;
}

PointLocation GetPointLocationBinary(const std::vector<Point>& polygon, Point P)
{
    int baseIndex = 0;
    int leftIndex = polygon.size() - 1;
    int rightIndex = baseIndex + 1;

    Point B = polygon[baseIndex];
    while (leftIndex - rightIndex > 1)
    {
        int mid = leftIndex + (rightIndex - leftIndex) / 2;

        Point L = polygon[leftIndex];
        Point M = polygon[mid];
        Point R = polygon[rightIndex];
        PointLocation locationLeft = GetPointLocationFromAngle(L, B, M, P);
        PointLocation locationRight = GetPointLocationFromAngle(M, B, R, P);
        if (locationLeft == PointLocation::Inside)
        {
            rightIndex = mid;
        }
        else if (locationRight == PointLocation::Inside)
        {
            leftIndex = mid;
        }
        else if (locationLeft == PointLocation::Edge || locationRight == PointLocation::Edge)
        {
            if ((locationLeft == PointLocation::Edge && leftIndex == polygon.size() - 1) || (locationRight == PointLocation::Edge && rightIndex == baseIndex + 1))
            {
                // Its the first try, so we are testing against edges of the polygon
                return PointLocation::Edge;
            }

            // Edge of a triangle formed from the vertices of a polygon is inside the polygon
            return PointLocation::Inside;
        }
        else
        {
            return PointLocation::Outside;
        }
    }

    Point L = polygon[leftIndex];
    Point R = polygon[rightIndex];
    PointLocation triangleLocation = GetPointLocationFromAngle(L, B, R, P);
    if (triangleLocation == PointLocation::Edge)
    {
        return PointLocation::Inside;
    }

    Orientation polygonOrientation = GetPolygonOrientation(polygon);
    Orientation triangleOrientation = GetOrientation(L, P, R);
    return polygonOrientation == triangleOrientation ? PointLocation::Inside : PointLocation::Outside;
}

int main()
{
    // TestCase Inside:     7 0 0 10 -5 20 0 20 10 17 20 14 20 0 10 5 5
    // TestCase Edge:       7 0 0 10 -5 20 0 20 10 17 20 14 20 0 10 0 0
    // TestCase Edge:       7 0 0 10 -5 20 0 20 10 17 20 14 20 0 10 5 -2.5
    // TestCase Outside:    7 0 0 10 -5 20 0 20 10 17 20 14 20 0 10 25 25

    //int n;
    //std::cin >> n;

    //std::vector<Point> points;
    //for (int i = 0; i < n; i++)
    //{
    //    Point point;
    //    std::cin >> point;
    //    points.push_back(point);
    //}

    //Point pointToCheck;
    //std::cin >> pointToCheck;

    /*std::vector<Point> points = {
        { 0, 0 },
        { 10, -5 },
        { 20, 0 },
        { 20, 10 },
        { 17, 20 },
        { 14, 20 },
        { 0, 10 },
    };*/

    std::vector<Point> points = {
       Point(0, 10),
       Point(14, 20),
       Point(17, 20),
       Point(20, 10),
       Point(20,  0),
       Point(10, -5),
       Point(0,  0),
    };
    Point pointToCheck(5, 5);

    {
        PointLocation location = GetPointLocationLinear(points, pointToCheck);
        std::cout << ToString(location) << "\n";
    }

    {
        PointLocation location = GetPointLocationBinary(points, pointToCheck);
        std::cout << ToString(location) << "\n";
    }


}