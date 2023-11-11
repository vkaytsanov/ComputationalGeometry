#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

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

int GetLeftMostPoint(const std::vector<Point>& points)
{
    int leftMostPointIndex = 0;
    for (int i = 1; i < points.size(); i++)
    {
        if (points[leftMostPointIndex].x > points[i].x)
        {
            leftMostPointIndex = i;
        }
    }
    return leftMostPointIndex;
}


std::vector<Point> GiftWrap_Jarvis(const std::vector<Point>& points)
{
    std::vector<Point> convexHull;

    int leftMostPointIndex = GetLeftMostPoint(points);
    int pointIndex = leftMostPointIndex;
    do
    {
        convexHull.emplace_back(points[pointIndex]);

        int nextPointIndex = (pointIndex + 1) % points.size();
        double biggestArea = DBL_MIN;
        for (int i = 0; i < points.size(); i++)
        {
            double area = GetAreaFromPoints(points[pointIndex], points[i], points[nextPointIndex]);
            if (area > biggestArea)
            {
                area = biggestArea;
                nextPointIndex = i;
            }
        }

        pointIndex = nextPointIndex;

    } while (pointIndex != leftMostPointIndex);

    return convexHull;
}

std::vector<Point> GrahamScan_Graham(const std::vector<Point>& points)
{
    std::vector<Point> convexHull;

    int leftMostPointIndex = GetLeftMostPoint(points);

    auto compareByAngle = [&](Point A, Point B)
    {
        Point P = points[leftMostPointIndex];
        Orientation orientation = GetOrientation(P, A, B);
        if (orientation == Orientation::Colinear)
        {
            double APDist = std::abs(A.x - P.x) + std::abs(A.y - P.y);
            double BPDist = std::abs(B.x - P.x) + std::abs(B.y - P.y);
            return APDist > BPDist;
        }
        return orientation == Orientation::CounterClockWise;
    };
    std::vector<Point> sortedPointsByAngle = points;
    std::swap(sortedPointsByAngle[0], sortedPointsByAngle[leftMostPointIndex]);
    std::sort(sortedPointsByAngle.begin() + 1, sortedPointsByAngle.end(), compareByAngle);

    for (Point P : sortedPointsByAngle)
    {
        while (convexHull.size() > 1 && GetOrientation(convexHull[convexHull.size() - 2], convexHull[convexHull.size() - 1], P) != Orientation::CounterClockWise)
        {
            convexHull.pop_back();
        }
        convexHull.push_back(P);
    }

    return convexHull;
}

std::vector<Point> MonotoneChain_Andrews(const std::vector<Point>& points)
{
    auto compareByXThenByY = [&](Point A, Point B)
    {
        if (A.x == B.x)
        {
            return A.y < B.y;
        }
        return A.x < B.x;
    };

    std::vector<Point> sortedPoints = points;
    std::sort(sortedPoints.begin(), sortedPoints.end(), compareByXThenByY);

    std::vector<Point> lowerChain;
    for (Point P : sortedPoints)
    {
        while (lowerChain.size() > 1 && GetOrientation(lowerChain[lowerChain.size() - 2], lowerChain[lowerChain.size() - 1], P) != Orientation::CounterClockWise)
        {
            lowerChain.pop_back();
        }

        lowerChain.push_back(P);
    }

    std::vector<Point> upperChain;
    for (int i = sortedPoints.size() - 1; i > 0; i--)
    {
        Point P = sortedPoints[i];
        while (upperChain.size() > 1 && GetOrientation(upperChain[upperChain.size() - 2], upperChain[upperChain.size() - 1], P) != Orientation::CounterClockWise)
        {
            upperChain.pop_back();
        }

        upperChain.push_back(P);
    }

    lowerChain.pop_back();
    upperChain.pop_back();

    // Concatanate
    for (Point P : upperChain)
    {
        lowerChain.push_back(P);
    }

    return lowerChain;
}


void PrintResult(const std::vector<Point>& result)
{
    for (Point P : result)
    {
        printf("[%.1lf, %.1lf] ", P.x, P.y);
    }

    printf("\n");
}

int main()
{
    std::vector<Point> points = {
        Point(0, 0), Point(10, 12), Point(15, 15), Point(20, 12), Point(25, 5), Point(19, -5), Point(10, -7),
        Point(12, 0), Point(11, 7), Point(10, 9), Point(13, 7), Point(14, 5)
    };

    PrintResult(GiftWrap_Jarvis(points));
    PrintResult(GrahamScan_Graham(points));
    PrintResult(MonotoneChain_Andrews(points));
}