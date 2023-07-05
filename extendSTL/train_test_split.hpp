#pragma once
#include "extendVector.hpp"
#include <utility>
#include <random>
#include <algorithm>

/*
* return pair<vector, vector>, first is train, second is test
*
* vector typed pair<T1, T2>, first is feature, second is target
*
*/
template<class T1, class T2>
std::pair<std::vector<std::pair<T1, T2>>, std::vector<std::pair<T1, T2>>>
train_test_split(
	std::vector<std::pair<T1, T2>>& datas,
	double trainSize = 0.7
)
{
	/*
	* provide random seed
	*/
	std::random_device rd;
	/*
	* visual random engine
	*/
	std::mt19937 rng(rd());

	/*
	* std::shuffle need mutable argument
	* const std::vector<> will compilation error
	*/
	std::shuffle(datas.begin(), datas.end(), rng);

	size_t mid = datas.size() * trainSize;

	return std::make_pair(splitVector<std::pair<T1, T2>>(datas, 0, mid),
		splitVector<std::pair<T1, T2>>(datas, mid + 1, datas.size() - 1));
}

/*
* right value reference
*/
template<class T1, class T2>
std::pair<std::vector<std::pair<T1, T2>>, std::vector<std::pair<T1, T2>>>
train_test_split(
	std::vector<std::pair<T1, T2>>&& datas,
	double trainSize = 0.7
)
{
	/*
	* provide random seed
	*/
	std::random_device rd;
	/*
	* visual random engine
	*/
	std::mt19937 rng(rd());

	std::shuffle(datas.begin(), datas.end(), rng);

	size_t mid = datas.size() * trainSize;

	return std::make_pair(splitVector<std::pair<T1, T2>>(datas, 0, mid),
		splitVector<std::pair<T1, T2>>(datas, mid + 1, datas.size() - 1));
}