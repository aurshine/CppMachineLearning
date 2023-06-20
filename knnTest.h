#pragma once
#include <fstream>
#include "KNearestNeighbor.hpp"

inline void knnTest()
{
	std::ifstream in;
	in.open("dataSets/knnDatingTestSet2.txt", std::ios::in | std::ios::app);

	using dataType = std::vector<std::pair<std::vector<double>, int>>;
	dataType datas;

	double f1, f2, f3;
	int t;
	while (in >> f1 >> f2 >> f3 >> t)
	{
		datas.push_back({ std::vector<double>{f1, f2, f3}, t });
		// std::cout << datas.back().first << " " << datas.back().second << std::endl;
	}

	//std::random_shuffle(datas.begin(), datas.end());
	int mid = datas.size() * 0.7;
	dataType train = splitVector(datas, 0, mid), test = splitVector(datas, mid + 1, datas.size() - 1);

	KNearestNeighbor<double, int> knn(train);

	std::cout << knn.score(test);
	in.close();
}