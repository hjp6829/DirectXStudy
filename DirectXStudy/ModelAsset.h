#pragma once
#include <unordered_map>
#include <string>

class ModelNode;

class ModelAsset {
public:
	ModelAsset() {}
	~ModelAsset() {}
	ModelNode* currentNode;
	void SetModelNode(uint64_t modelLocalHesh, ModelNode* node){ modelNodesDic.insert({ modelLocalHesh, node }); }
	ModelNode* GetModelNode(uint64_t guid);
private:
	std::unordered_map<uint64_t, ModelNode*> modelNodesDic;
};