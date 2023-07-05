#pragma once
#include "extendVector.hpp"
#include <cmath>

template<class T>
double EuclideanDist(const std::vector<T>& v1, const std::vector<T>& v2)
{
	double dist = 0;
	for (size_t i = 0; i < v1.size() || i < v2.size(); i++)
	{
		auto res = atVector(v1, i) - atVector(v2, i);
		dist += res * res;
	}

	return sqrt(dist);
}

template<class T>
double ManhattanDist(const std::vector<T>& v1, const std::vector<T>& v2)
{
	double dist = 0;
	for (size_t i = 0; i < v1.size() || i < v2.size(); i++)
		dist += abs(atVector(v1, i) - atVector(v2, i));

	return dist;
}

template<class T>
double ChebyshevDist(const std::vector<T>& v1, const std::vector<T>& v2)
{
	double dist = 0;
	for (size_t i = 0; i < v1.size() || i < v2.size(); i++)
	{
		if (dist == 0)
			dist = abs(atVector(v1, i) - atVctor(v2, i));
		else
			dist = max(dist, abs(atVector(v1, i) - atVctor(v2, i)));
	}

	return 0;
}
