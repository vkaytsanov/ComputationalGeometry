#include <iostream>
#include "vecta.h"

typedef vecta::vec2d<double> Point;

template <typename T>
T GetAreaFromPoints(vecta::vec2d<T> a, vecta::vec2d<T> b, vecta::vec2d<T> p)
{
	vecta::vec2d<double> AB = b - a;
	vecta::vec2d<double> AP = p - a;
	return AB ^ AP;
}

/// TestCase Line    1: 0 0 10 0 5 5 2.5 2.5
/// TestCase Inside  2: 0 0 10 0 5 5 5 2.5
/// TestCase Outside 3: 0 0 10 0 5 5 0 1
/// TestCase Outside 4: 0 0 10 0 5 5 5 -1
/// TestCase Edge    5: 0 0 10 0 5 5 0 0
int main()
{
	Point A, B, C, P;
	std::cin >> A >> B >> C >> P;

	double ABP = GetAreaFromPoints(A, B, P);
	double BCP = GetAreaFromPoints(B, C, P);
	double CAP = GetAreaFromPoints(C, A, P);

	std::cout << "Area ABP:" << ABP << "\n"
	          << "Area BCP:" << BCP << "\n"
	          << "Area CAP:" << CAP << "\n";

	if ((ABP > 0 && BCP > 0 && CAP > 0) || (ABP < 0 && BCP < 0 && CAP < 0))
	{
		std::cout << "Point is inside triangle\n";
	}
	else
	{
		int zeros = 0;
		if (ABP == 0) zeros++;
		if (BCP == 0) zeros++;
		if (CAP == 0) zeros++;

		if (zeros == 2)
		{
			std::cout << "Point is at edge\n";
		}
		else if (zeros == 1)
		{
			std::cout << "Point is at a line\n";
		}
		else
		{
			std::cout << "Point is outside triangle\n";
		}
	}
}
