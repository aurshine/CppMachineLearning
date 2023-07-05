#pragma once
#include <map>
#include <utility>
#include <functional>

template <class K, class V>
bool maxValue(std::pair<K, V> p1, std::pair<K, V> p2)
{
	return p1.second > p2.second;
}

template <class K, class V>
std::pair<K, V> maxInMap(const std::map<K, V>& mp,
	std::function<bool(std::pair<K, V> p1, std::pair<K, V> p2)> f = maxValue<K, V>)
{
	bool flag = false;
	std::pair<K, V> maxv{};

	for (auto iter : mp)
	{
		if (!flag)
		{
			maxv = iter;
			flag = true;
		}

		if (!f(maxv, iter))
			maxv = iter;
	}

	return maxv;
}