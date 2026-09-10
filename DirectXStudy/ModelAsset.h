#pragma once
#include <vector>
class ModelNode;

class ModelAsset {
public:
	ModelAsset() {}
	~ModelAsset() {}
	ModelNode* currentNode;
	std::unordered_map<uint64_t, ModelNode*> modelNodesDic;
};