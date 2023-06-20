#pragma once
#include <cmath>
#include <algorithm>
#include <map>
#include <functional>
#include "extendVector.hpp"

template<class T>
double clacEuclideanDist(const std::vector<T>& v1, const std::vector<T>& v2)
{
	double dist = 0;
	for (size_t i = 0; i < v1.size() || i < v2.size(); i++)
	{
		auto res = atVector(v1, i) - atVector(v2, i);
		dist += res * res;
	}

	return sqrt(dist);
}

template<class featureType = double, class targetType = double>
class KNearestNeighbor
{
	using T1 = std::vector<featureType>;
	using T2 = targetType;

public:
	/*
	* constructor for TestSets
	*/
	KNearestNeighbor(const std::vector<T1>& feature, const std::vector<T2>& target)
		:feature(feature), target(target)
	{

	}

	KNearestNeighbor(const std::vector<std::pair<T1, T2>>& data)
	{
		for (auto p : data)
		{
			this->feature.push_back(p.first);
			this->target.push_back(p.second);
		}
	}

	constexpr std::size_t size() const
	{
		return this->feature.size();
	}

	/*
	* return at most K Nearest pair{distance, target}
	*/
	std::vector<std::pair<double, T2>> KNearest(const T1& m_feature,
		int K = 5,
		std::function<double(const std::vector<featureType>&, const std::vector<featureType>&)> clacDistance = clacEuclideanDist<featureType>) const
	{
		using PDT = std::pair<double, T2>;
		std::vector<PDT> kNearest;

		/*
		* push each distance train feature with m_feature and train target
		*/
		for (auto i = 0; i < feature.size(); i++)
			kNearest.push_back({ clacDistance(feature[i], m_feature), this->target[i] });

		/*
		* only one keyword of sort by lambda
		*/
		sort(kNearest.begin(), kNearest.end(), [](PDT p1, PDT p2) {return p1.first < p2.first;});

		/*
		* shrink vector to get K-Nearest feature
		*/
		shrinkVector(kNearest, K);

		return kNearest;
	}

	/*
	* predict feature in which target
	*/
	T2 predict(const T1& feature,
		int K = 5,
		std::function<double(const std::vector<featureType>&, const std::vector<featureType>&)> clacDistance = clacEuclideanDist<featureType>)	const
	{
		/*
		* record the count of targets
		* first of target
		* second of count
		*/
		std::map<T2, size_t> targetCount;

		/*
		* return at most K Nearest pair{distance, target}
		*/
		auto kNearest = this->KNearest(feature, K, clacDistance);

		std::vector<T2> predictTarget;
		for (auto iter : kNearest)
		{
			if (targetCount[iter.second] ++ == 0)
				predictTarget.push_back(iter.second);
		}

		auto tcCmp = [&targetCount](T2 a, T2 b) {
			return targetCount[a] > targetCount[b];
		};
		std::sort(predictTarget.begin(), predictTarget.end(), tcCmp);

		if (predictTarget.size() < 2 || tcCmp(predictTarget[0], predictTarget[1]))
			return predictTarget[0];
		else
		{
			predictTarget = filterVector<T2>(predictTarget, [&predictTarget](T2 t) {return predictTarget[0] == t;});
			std::vector<double> predictDistance(predictTarget.size());

			for (size_t i = 0; i < kNearest.size(); i++)
			{
				auto idx = binSearchVector<T2>(predictTarget, kNearest[i].second, tcCmp);
				if (~idx == 0)	continue;

				predictDistance[idx] += kNearest[i].first;
			}

			return predictTarget[bestInVector<double>(predictDistance, [](double a, double b) {return a > b;})];
		}
	}

	std::vector<T2> predict(const std::vector<T1>& features,
		int K = 5,
		std::function<double(const std::vector<featureType>&, const std::vector<featureType>&)> clacDistance = clacEuclideanDist<featureType>)	const
	{
		std::vector<T2> predictTargets;
		for (auto iter : features)
			predictTargets.push_back(this->predict(iter, K, clacDistance));

		return predictTargets;
	}

	/*
	* Claculate the accuracy of the test set
	*/
	double score(const std::vector<std::pair<T1, T2>>& data,
		int K = 5,
		std::function<double(const std::vector<featureType>&, const std::vector<featureType>&)> clacDistance = clacEuclideanDist<featureType>)	const
	{
		size_t cnt = 0;
		for (int i = 0; i < data.size(); i++)
			cnt += (this->predict(data[i].first, K, clacDistance) == data[i].second);

		return 1.0 * cnt / data.size();
	}

	double score(const std::vector<T1>& features,
		const std::vector<T2>& targets,
		int K = 5,
		std::function<double(const std::vector<featureType>&, const std::vector<featureType>&)> clacDistance = clacEuclideanDist<featureType>)	const
	{
		std::vector<T2> predictTargets = this->predict(features, K, clacDistance);

		size_t cnt = 0;
		for (int i = 0; i < predictTargets.size(); i++)
			cnt += predictTargets[i] == targets[i];

		return 1.0 * cnt / target.size();
	}

private:
	std::vector<T1> feature;
	std::vector<T2> target;
};