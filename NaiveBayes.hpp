#pragma once
#include "extendVector.hpp"
#include "PrefixSum.hpp"
#include "extendMap.hpp"
#include <cmath>

/*
* Conditional probability p(A | B)
* The probability of event A occurring given the condition of event B.
*
* feature = {f1, f2 ,f3, ..., fn}
*
* p(target | feature) = p(target) * p(f1 & f2 & f3 ... fn | target) / (p(f1) * p(f2) * p(f3) * ... * p(fn))
*
* f1 f2 f3 ... fn are mutually independent.
*
* p(target | feature) = p(target) * p(f1 | target) * p(f2 | target) * p(f3 | target) * ... * p(fn | target) / (p(f1) * p(f2) * p(f3) * ... * p(fn))
*
* p(f1) p(f2) ... p(fn) are constant
*
* for each target
*
* we can only claculate p(target) * p(f1 | target) * ... * p(fn | target);
*/
template<class featureType, class targetType>
class NaiveBayes
{
	using T1 = std::vector<featureType>;
	using T2 = targetType;

public:
	NaiveBayes(const std::vector<T1>& features, const std::vector<T2>& targets)
	{
		auto uniqueTarget = frequencyVector(targets);

		for (auto target : uniqueTarget)
		{
			/*
			* filter the feature which feature_target == target.first
			*/
			std::vector<T1> filtered = filterIndex(features, [&targets, &target](size_t idx) {return targets[idx] == target.first;});

			this->optionProbability[target.first] = this->BayesTargetCategory(filtered);
			this->targetsProbability[target.first] = 1.0 * target.second / targets.size();
		}
	}

	NaiveBayes(const std::vector<std::pair<T1, T2>>& datas)
	{
		auto frequencyFunction = [](std::pair<T1, T2> p) {
			return p.second;
		};

		auto uniqueTarget = frequencyVector<std::pair<T1, T2>, T2>(datas, frequencyFunction);

		for (auto target : uniqueTarget)
		{
			auto filtered = filterVector<std::pair<T1, T2>>(datas, [&target](std::pair<T1, T2> p) { return p.second == target.first;});
			this->optionProbability[target.first] = this->BayesTargetCategory(filtered);
			this->targetsProbability[target.first] = 1.0 * target.second / datas.size();
		}
	}

	T2 predict(const T1& feature)	const
	{
		std::map<T2, double>	predictProbability;
		for (auto target : this->targetsProbability)
		{
			predictProbability[target.first] = target.second;

			double& probability = predictProbability[target.first];
			auto& _optionProbability = this->optionProbability[target.first];

			for (size_t i = 0; i < feature.size(); ++i)
				probability *= _optionProbability[i][feature[i]];
		}

		return maxInMap(predictProbability).first;
	}

	T2 predict(const std::vector<T1>& features)	const
	{
		std::vector<T2> predictTarget(features.size());

		for (size_t i = 0; i < features.size(); ++i)
			predictTarget[i] = this->predict(features[i]);

		return predictTarget;
	}

	double score(const std::vector<T1>& features, const std::vector<T2>& targets)	const
	{
		size_t cnt = 0;
		for (size_t i = 0; i < features.size(); ++i)
			cnt += this->predict(features[i]) == targets[i];

		return 1.0 * cnt / targets.size();
	}

	double score(const std::vector<std::pair<T1, T2>> datas)	const
	{
		size_t cnt = 0;
		for (size_t i = 0; i < datas.size(); ++i)
			cnt += this->predict(datas[i].first) == datas[i].second;

		return 1.0 * cnt / datas.size();
	}
private:
	std::vector<std::map<featureType, double>> BayesTargetCategory(const std::vector<T1>& features)	const
	{
		std::vector<std::map<featureType, double>> result;

		if (features.empty())	return result;

		/*
		* enumerate each options from feature[0] to feature[n]
		*/
		for (size_t i = 0; i < features[0].size(); ++i)
		{
			auto filterFunction = [i](T1 feature) {
				return feature[i];
			};

			/*
			* return map{option, count}
			*/
			auto optionFrequency = frequencyVector<T1, featureType>(features, filterFunction);

			/*
			* claculate probability of occurrence of each option
			*/
			std::map<featureType, double>	probability;
			for (auto option : optionFrequency)
				probability[option.first] = 1.0 * option.second / features.size();

			result.push_back(probability);
		}

		return result;
	}

	std::vector<std::map<featureType, double>> BayesTargetCategory(const std::vector<std::pair<T1, T2>> datas)	const
	{
		std::vector<std::map<featureType, double>> result;

		if (datas.empty())	return result;

		/*
		* enumerate each options from feature[0] to feature[n]
		*/
		for (size_t i = 0; i < datas[0].first.size(); ++i)
		{
			auto filterFunction = [i](std::pair<T1, T2> p) {
				return p.first[i];
			};

			/*
			* return map{option, count}
			*/
			auto optionFrequency = frequencyVector<std::pair<T1, T2>, featureType>(datas, filterFunction);

			/*
			* claculate probability of occurrence of each option
			*/
			std::map<featureType, double>	probability;
			for (auto option : optionFrequency)
				probability[option.first] = 1.0 * option.second / datas.size();

			result.push_back(probability);
		}

		return result;
	}
private:
	std::map<T2, double> targetsProbability;
	mutable std::map<T2, std::vector<std::map<featureType, double>>> optionProbability;
};