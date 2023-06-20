#pragma once
#include "extendVector.hpp"
#include <map>
#include <memory>
#include <cassert>

template<class featureType, class targetType>
class DecisionNode
{
	using nodePtr = std::shared_ptr<DecisionNode>;

public:
	/*
	* feature is edge
	* ptr is next node
	*/
	void to(featureType feature, nodePtr ptr)
	{
		nextNodes[feature] = ptr;
	}

	// index in feature-vector
	size_t nextFeature;

	// The target corresponding to the current node.
	targetType target;

	// storage edges
	std::map<featureType, nodePtr> nextNodes;
};

