#include "costFunction.h"

/*
* y = kx + b
*/
double yLabel(
	const std::vector<double>& x,
	const std::vector<double>& k,
	double b)
{
	double y = b;
	for (size_t i = 0; i < x.size(); ++i)
		y += k[i] * x[i];

	return y;
}

/*
* claculate value difference between real y and predict y
*/
double cost(
	const std::vector<std::vector<double>>& x,
	const std::vector<double>& y,
	const std::vector<double>& k,
	double b
)
{
	double res = 0;
	for (size_t i = 0; i < y.size(); i++)
		res += abs(yLabel(x[i], k, b) - y[i]);

	return res;
}

double cost(
	const std::vector<std::pair<std::vector<double>, double>>& xy,
	const std::vector<double>& k,
	double b
)
{
	double res = 0;
	for (int i = 0; i < xy.size(); ++i)
		res += abs(yLabel(xy[i].first, k, b) - xy[i].second);

	return res;
}