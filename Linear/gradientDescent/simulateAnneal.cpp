#include "simulateAnneal.h"


double rand(double l, double r)
{
	return (double)rand() / RAND_MAX * (r - l) + l;
}


/*
* simulateAnneal claculate coefficient and intercept
* xLabel values
* ylabel values
* Attenuation coefficient
* begin temperature
* end temperature
*/
std::pair<std::vector<double>, double> simulateAnneal(
	const std::vector<std::vector<double>>& x,
	const std::vector<double>& y,
	double alpha,
	double beginT,
	double endT
)
{
	double b = 0, newB = 0;
	std::vector<double> k(x[0].size(), 0), newK(k.size(), 0);

	for (double T0 = beginT; T0 > endT; T0 -= T0 * alpha)
	{
		/*
		* search a new solution within neighborhood
		*/
		newB = rand(b - T0, b + T0);
		for (size_t i = 0; i < k.size(); ++i)
			newK[i] = rand(k[i] - T0, k[i] + T0);

		double dt = cost(x, y, newK, newB) - cost(x, y, k, b);
		if (exp(-dt / T0) > rand(0, 1.0))	k = newK, b = newB;
	}

	return { k, b };
}

std::pair<std::vector<double>, double> simulateAnneal(
	const std::vector<std::pair<std::vector<double>, double>>& xy,
	double alpha,
	double beginT,
	double endT
)
{
	double b = 0, newB = 0;
	std::vector<double> k(xy[0].first.size(), 0), newK(k.size(), 0);

	for (double T0 = beginT; T0 > endT; T0 -= T0 * alpha)
	{
		/*
		* search a new solution within neighborhood
		*/
		newB = rand(b - T0, b + T0);
		for (size_t i = 0; i < k.size(); ++i)
			newK[i] = rand(k[i] - T0, k[i] + T0);

		double dt = cost(xy, newK, newB) - cost(xy, k, b);
		if (exp(-dt / T0) > rand(0, 1.0))	k = newK, b = newB;
	}

	return { k, b };
}