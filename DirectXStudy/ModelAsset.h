#pragma once
#include <unordered_map>

class ModelNode;

class ModelAsset {
public:
	ModelAsset() {}
	~ModelAsset() {}
	ModelNode* currentNode;
	void SetModelNode(std::string guid, ModelNode* node){ modelNodesDic.insert({ guid, node }); }
	ModelNode* GetModelNode(std::string guid);
private:
	std::unordered_map<std::string, ModelNode*> modelNodesDic;
};