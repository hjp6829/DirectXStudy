#pragma once
#include <unordered_map>
#include <string>
#include "Datas.h"

class ModelNode;

class ModelAsset {
public:
	ModelAsset() {}
	~ModelAsset() {}
	ModelNode* currentNode;
	void SetModelNode(uint64_t modelLocalHesh, ModelNode* node){ modelNodesDic.insert({ modelLocalHesh, node }); }
	ModelNode* GetModelNode(uint64_t guid);
	void SetMaterialTexture(int matertialIDX, TextureAsset* textureAsset, aiTextureType type);
	std::unordered_map<int, Material*>* GetMaterialsDic() { return &modelMaterialsDic; }
private:
	std::unordered_map<uint64_t, ModelNode*> modelNodesDic;
	std::unordered_map<int, Material*> modelMaterialsDic;
};