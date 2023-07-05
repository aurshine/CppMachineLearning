#pragma once
#include "LinearRegression.hpp"
#include "simulateAnneal.h"

inline void linearRegressionTest()
{
	std::vector<double> k{ 1, 2, 3, 4, 5 };
	double b = 6;

	std::vector<std::vector<double>> x(100, std::vector<double>(5));
	std::vector<double> y(100);
	for (int i = 0; i < x.size(); i++)
	{
		y[i] = b;
		for (int j = 0; j < 5; j++)
		{
			x[i][j] = rand() % 10;
			y[i] += x[i][j] * k[j];
		}
	}

	auto reg = LinearRegression(x, y);
	std::cout << reg._coef() << " " << reg._intercept() << "\n";
	std::cout << "cost = " << reg._cost(x, y) << "\n";
	std::cout << "predict {2, 2, 2, 2, 2}" << reg.predict(std::vector<double>({ 2, 2, 2, 2, 2 }));
}
