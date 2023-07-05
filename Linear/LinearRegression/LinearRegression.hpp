#include "extendVector.hpp"
#include "simulateAnneal.h"

class LinearRegression
{
	using T1 = std::vector<double>;
	using T2 = double;

public:
	/*
	* LinearRegression by simulateAnneal
	*/
	LinearRegression(
		const std::vector<T1>& features,
		const std::vector<T2>& targets,
		size_t times = 100,
		double alpha = 0.01,
		double beginT = 100,
		double endT = 1e-4
	)
		:coef(targets.size())
	{
		std::pair<T1, T2> p = simulateAnneal(features, targets, alpha, beginT, endT);
		while (times--)
		{
			auto _p = simulateAnneal(features, targets, alpha, beginT, endT);
			if (cost(features, targets, p.first, p.second) > cost(features, targets, _p.first, _p.second))
				p = _p;
		}

		this->coef = p.first;
		this->intercept = p.second;
	}

	LinearRegression(
		const std::vector<std::pair<T1, T2>>& datas,
		size_t times = 100,
		double alpha = 0.01,
		double beginT = 100,
		double endT = 1e-4
	)
		:coef(datas.size())
	{
		std::pair<T1, T2> p = simulateAnneal(datas, alpha, beginT, endT);
		while (times--)
		{
			auto _p = simulateAnneal(datas, alpha, beginT, endT);
			if (cost(datas, p.first, p.second) > cost(datas, _p.first, _p.second))
				p = _p;
		}

		this->coef = p.first;
		this->intercept = p.second;
	}

	double predict(const T1& feature)	const
	{
		return yLabel(feature, coef, intercept);
	}

	/*
	* y = kx + b -> k
	*/
	T1 _coef()	const
	{
		return this->coef;
	}

	/*
	* y = kx + b -> b
	*/
	T2 _intercept()	const
	{
		return this->intercept;
	}

	/*
	* the value difference between real y and predict y
	*/
	double _cost(
		const std::vector<T1>& features,
		const std::vector<T2>& targets)
	{
		return cost(features, targets, this->coef, this->intercept);
	}

private:
	T1 coef;
	T2 intercept;
};
