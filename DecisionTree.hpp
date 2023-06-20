#pragma once
#include <algorithm>
#include <functional>
#include <map>
#include <cmath>
#include "DecisionNode.hpp"
#include "extendVector.hpp"

template<class T>
double GiniIndex(const std::vector<T>& targets)
{
	std::map<T, size_t> targetCount = frequencyVector<T>(targets);

	double giniIndex = 1;

	for (auto count : targetCount)
	{
		double p = 1.0 * count.second / targets.size();
		giniIndex -= p * p;
	}

	return giniIndex;
}

template <class T>
double ShannonEnt(const std::vector<T>& targets)
{
	std::map<T, size_t> targetCount = frequencyVector<T>(targets);

	double shannonEnt = 0;

	for (auto count : targetCount)
	{
		double p = 1.0 * count.second / targets.size();

		shannonEnt -= p * log(p) / log(2);
	}

	return shannonEnt;
}

template<class featureType = double, class targetType = double>
class DecisionTree
{
	using T1 = std::vector<featureType>;
	using T2 = targetType;
	using Node = DecisionNode<featureType, targetType>;

public:
	DecisionTree(const std::vector<T1>& feature,
		const std::vector<T2>& target,
		std::function<double(const std::vector<T2>&)> chaos = GiniIndex<T2>)
		: root(this->build(feature, target, chaos))
	{

	}

	DecisionTree(const std::vector<std::pair<T1, T2>>& data,
		std::function<double(const std::vector<T2>&)> chaos = GiniIndex<T2>)
	{
		std::vector<T1> feature(data.size());
		std::vector<T2> target(data.size());

		for (size_t i = 0; i < data.size(); ++i)
		{
			feature[i] = data[i].first;
			target[i] = data[i].second;
		}

		this->root = this->build(feature, target, chaos);
	}

	T2 predict(const T1& feature, size_t maxDepth = 10)
	{
		std::shared_ptr<Node> currentRoot = this->root;
		T2 predictTarget;
		for (size_t i = 0; i <= maxDepth && currentRoot; ++i)
		{
			predictTarget = currentRoot->target;

			if (currentRoot->nextFeature == -1)	break;
			currentRoot = currentRoot->nextNodes[feature[currentRoot->nextFeature]];
		}

		return predictTarget;
	}

	std::vector<T2> predict(const std::vector<T1>& features, size_t maxDepth = 10)
	{
		std::vector<T2> predictTargets(features.size());
		for (size_t i = 0; i < features.size(); ++i)
			predictTargets[i] = this->predict(features[i], maxDepth);

		return predictTargets;
	}

	double score(const std::vector<T1>& feature,
		const std::vector<T2>& target,
		size_t maxDepth = 10)
	{
		size_t cnt = 0;
		for (size_t i = 0; i < feature.size(); ++i)
			cnt += this->predict(feature[i], maxDepth) == target[i];

		return 1.0 * cnt / feature.size();
	}

	double score(const std::vector<std::pair<T1, T2>>& data,
		size_t maxDepth = 10)
	{
		size_t cnt = 0;
		for (auto p : data)
			cnt += this->predict(p.first) == p.second;

		return 1.0 * cnt / data.size();
	}
private:

	/*
	* build function it a recursive function
	* "Feature" and "target" refer to the data that is currently available
	* choas function used to calculate chaos value for each feature
	*
	*/
	std::shared_ptr<Node> build(const std::vector<T1>& feature,
		const std::vector<T2>& target,
		std::function<double(const std::vector<T2>&)> chaos = GiniIndex<T2>)	const
	{
		/*
		* create new node
		*/
		std::shared_ptr<Node> root{ new Node };

		/*
		* claculate the best feature to next node
		* it return column of best feature in current data
		* return size_t
		*/
		root->nextFeature = this->bestFeature(feature, target, chaos);
		root->target = frequencyVector<T2>(target).rbegin()->first;

		if (root->nextFeature == -1)	return root;
		/*
		* claculate the highest frequency target in curent data
		* return map{option, count}
		*/

		auto column = root->nextFeature;
		/*
		* how many options a available for selection
		* return map{option in each column, count}
		*/
		auto options = frequencyVector<T1, featureType>(feature, [column](T1 v) {return v[column];});
		/*
		* connect edge by each option
		*/
		for (auto option : options)
		{
			/*
			* filter all feature in the column that are equals to option.first
			*/
			auto newFeature = filterVector<T1>(feature, [&option, column](T1 f) {return f[column] == option.first;});
			/*
			* filter the target correspond to the feature
			*/
			auto newTarget = filterIndex<T2>(target, [&option, column, &feature](size_t idx) {return feature[idx][column] == option.first;});
			/*
			* drop column
			*/
			for (size_t i = 0; i < newFeature.size(); ++i)
				newFeature[i] = dropVector<featureType>(newFeature[i], column);

			std::shared_ptr<Node> newNode = this->build(newFeature, newTarget, chaos);
			// std::cout << option.first << " " << newTarget << " " << newFeature << "\n";
			/*
			* edge and next node
			*/
			root->to(option.first, newNode);
		}

		return root;
	}

	size_t bestFeature(const std::vector<T1>& feature,
		const std::vector<T2>& target,
		std::function<double(const std::vector<T2>&)> chaos = GiniIndex<T2>)	const
	{
		if (feature.size() == 0)	return -1;
		/*
		* the current best value/ID of chaos
		*/
		double bestChaos = 1e9;
		size_t bestID = -1, category = feature[0].size();

		/*
		* enumerate each feature in vector
		*/
		for (size_t i = 0; i < category; i++)
		{
			/*
			* in i-th feature, how many options it can select
			*
			* uniqueValue  -> map{options, count}
			*/
			auto uniqueValue = frequencyVector<T1, featureType>(feature, [i](T1 f) {return f[i];});

			/*
			* sum of the chaos value of all options
			*/
			double currentChaos = 0;
			for (auto iter : uniqueValue)
			{
				auto filter = filterIndex(target, [&iter, &feature, i](size_t idx) {return iter.first == feature[idx][i];});
				currentChaos += chaos(filter) * (1.0 * filter.size() / feature.size());
			}

			if (currentChaos < bestChaos)
			{
				bestChaos = currentChaos;
				bestID = i;
			}
		}

		return bestID;
	}

private:
	std::shared_ptr<Node> root;
};