#pragma once
#include "extendVector.hpp"

template<class T = size_t>
class PrefixSum
{
public:
	PrefixSum()
		:prefix(0) {}

	PrefixSum(size_t size, T defVal = T())
		:prefix(size, defVal) {}

	PrefixSum(const std::vector<T>& vec)
		:prefix(vec.size())
	{
		for (size_t i = 0; i < prefix.size(); ++i)
		{
			if (i == 0)
				prefix[i] = vec[i];
			else
				prefix[i] = prefix[i - 1] + vec[i];
		}
	}

	PrefixSum(const PrefixSum& pfs)
		:prefix(pfs.prefix) {}

	T query(size_t lpos, size_t rpos) const
	{
		if (lpos == 0)	return prefix[rpos];
		return prefix[rpos] - prefix[lpos - 1];
	}

	T query(size_t lpos) const
	{
		return this->query(lpos, lpos);
	}

	T operator[] (size_t index)	const
	{
		return this->query(index);
	}

	PrefixSum& operator= (const PrefixSum& pfs)
	{
		this->prefix = pfs.prefix;
		return *this;
	}

	size_t size() const
	{
		return prefix.size();
	}

	void add(const T& x, size_t pos)
	{
		this->add(x, pos, pos);
	}

	void add(const T& x, size_t lpos, size_t rpos)
	{
		prefix[lpos] += x;
		if (rpos + 1 != prefix.size()) prefix[rpos + 1] -= x;
	}

	T sum()	const
	{
		return prefix.back();
	}

	void push_back(const T& x)
	{
		prefix.push_back(x + prefix.back());
	}

	void resize(size_t newSize)
	{
		prefix.resize(newSize);
	}

private:
	std::vector<T> prefix;
};