#pragma once
#include <vector>
#include <unordered_map>
#include <d3d11.h>
#include <string>
#include <filesystem>

class ModelNode;
class ModelAsset;
class Object;
class ModelLoadData;
class ScriptRegistry;
class ObjectManager;
struct MetaData;

class ModelCreater
{
public:
	ModelCreater(ID3D11Device* device);
	ModelAsset* CreateModelAsset(ModelLoadData* modelData, const MetaData* metaData);
	void CreateChildModelNode(ModelLoadData* modelLoadData, ModelNode* modelNode, ModelAsset* modelAsset);
	ModelAsset* LoadModelFromFile(const MetaData* metaData, ModelLoadData* modelLoadData);
	ModelNode* GetModelNode(const MetaData* metaData, uint64_t localHesh);
private:
	std::unordered_map<std::string, ModelAsset*> modelAssets;
	ID3D11Device* device; 
	uint64_t FNV1a(const std::string& str);
	void SetModelNodeData(ModelNode* modelNode, ModelLoadData* modelLoadData);
};