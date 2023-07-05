#pragma once
#include <vector>

double yLabel(const std::vector<double>& x, const std::vector<double>& k, double b);

double cost(
	const std::vector<std::vector<double>>& x,
	const std::vector<double>& y,
	const std::vector<double>& k,
	double b
);

double cost(
	const std::vector<std::pair<std::vector<double>, double>>& xy,
	const std::vector<double>& k,
	double b
);
