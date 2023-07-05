#pragma once
#include "extendVector.hpp"
#include "costFunction.h"
#include <cmath>
#include <ctime>

double rand(double l, double r);

std::pair<std::vector<double>, double> simulateAnneal(
	const std::vector<std::vector<double>>& x,
	const std::vector<double>& y,
	double alpha,
	double beginT,
	double endT
);

std::pair<std::vector<double>, double> simulateAnneal(
	const std::vector<std::pair<std::vector<double>, double>>& xy,
	double alpha,
	double beginT,
	double endT
);