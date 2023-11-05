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

PointLocation GetPointLocationMonotone(const std::vector<Point>& upperChain, const std::vector<Point>& lowerChain, Point P)
{
    Point A = upperChain[0];
    Point B = upperChain[0];
    for (const Point& C : upperChain)
    {
        if (A.x > C.x)
        {
            A = C;
        }
        if (C.x > B.x)
        {
            B = C;
        }
    }

    for (const Point& C : lowerChain)
    {
        if (A.x > C.x)
        {
            A = C;
        }
        if (C.x > B.x)
        {
            B = C;
        }
    }

    Point u = B - A;

    Point AP = P - A;
    Point BP = P - B;

    if (u * AP < 0 || u * BP > 0)
    {
        return PointLocation::Outside;
    }

    // TODO: How it is done?
    //PointLocation upperSideLocation = GetPointLocationLinear(upperChain, P);
    //if (upperSideLocation != PointLocation::Inside)
    //{
    //    return upperSideLocation;
    //}

    //return GetPointLocationLinear(lowerChain, P);
}

int main()
{
    std::vector<Point> upperChain = {
        Point(0, 0), Point(5, 10), Point(10, 10), Point(15, 5), Point(20, 20), Point(25, 15)
    };

    std::vector<Point> lowerChain = {
        Point(3, 3), Point(6, 3), Point(17, 1), Point(22, 3), 
    };
    Point pointToCheck(1, 1);
    Point lineA(0, 0);
    Point lineB(30, 0);

    PointLocation location = GetPointLocationMonotone(upperChain, lowerChain, pointToCheck);
    std::cout << ToString(location) << "\n";
}