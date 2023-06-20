#pragma once
#include "DecisionTree.hpp"
#include <fstream>

inline void decisionTreeTest()
{
	std::ifstream in;
	in.open("D:\\PersonalFiles\\Github\\AI\\ailearning\\docs\\ml\\CppMachineLearning\\dataSets\\decisiontreeTestData.txt", std::ios::in);

	std::vector<std::vector<double>> features;
	std::vector<std::string> targets;

	double height, weight;
	std::string body;
	while (in >> height >> weight >> body)
	{
		features.push_back({ height, weight });
		targets.push_back(body);
	}

	DecisionTree<double, std::string> dtree(features, targets, ShannonEnt<std::string>);
	std::cout << dtree.predict({ 1.8, 60 }) << "\n";
}