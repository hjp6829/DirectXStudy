#pragma once
#include <unordered_map>

class ModelNode;

class ModelAsset {
public:
	ModelAsset() {}
	~ModelAsset() {}
	ModelNode* currentNode;
	void SetModelNode(uint64_t id, ModelNode* node){ modelNodesDic.insert({ id, node }); }
	ModelNode* GetModelNode(uint64_t id);
private:
	std::unordered_map<uint64_t, ModelNode*> modelNodesDic;
};