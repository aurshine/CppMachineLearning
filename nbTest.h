#pragma once
#include "NaiveBayes.hpp"
#include <fstream>

inline void nbTest()
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

	NaiveBayes<double, int> nb(unionVector(features, targets));
	std::cout << nb.score(unionVector(features, targets));
}