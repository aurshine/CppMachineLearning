#pragma once
#include "DecisionTree.hpp"
#include "train_test_split.hpp"
#include <fstream>

inline void decisionTreeTest()
{
	std::ifstream in;
	in.open("D:\\PersonalFiles\\Github\\AI\\ailearning\\docs\\ml\\CppMachineLearning\\dataSets\\NaiveBayes\\feature.txt", std::ios::in);

	std::vector<std::vector<double>> features;

	double f1, f2, f3, f4;
	while (in >> f1 >> f2 >> f3 >> f4)
		features.push_back({ f1, f2, f3, f4 });
	in.close();

	in.open("D:\\PersonalFiles\\Github\\AI\\ailearning\\docs\\ml\\CppMachineLearning\\dataSets\\NaiveBayes\\target.txt", std::ios::in);
	std::vector<int> targets;

	int t;
	while (in >> t)
		targets.push_back(t);

	auto data = unionVector(features, targets);
	auto train_test = train_test_split(data);

	DecisionTree<double, int> dtree(train_test.first);

	std::cout << dtree.score(train_test.second, 6);
}