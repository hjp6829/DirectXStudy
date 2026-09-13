#pragma once
#include <vector>
#include <unordered_map>
#include <d3d11.h>
#include <string>
#include <filesystem>

class ModelNode;
class ModelAsset;
class SceneModel;
class AssimpConverter;
class ModelLoadData;
struct JsonSceneModelData;

class ModelCreater
{
public:
	ModelCreater(ID3D11Device* device);
	ModelAsset* CreateModelAsset(ModelLoadData* modelData, std::filesystem::path modelName);
	void CreateChildModelNode(ModelLoadData* modelLoadData, ModelNode* modelNode, ModelAsset* modelAsset);
	SceneModel* CreateSceneModel(ModelAsset* modelAsset);
	void BuildSceneModelTree(ModelNode* modelNode, SceneModel* parentSceneModel);
	SceneModel* LoadModelFromFile(std::string path);
	SceneModel* CreateSceneModelFromJsonData(std::filesystem::path path, JsonSceneModelData& jsonModelData);
	SceneModel* CreateSingleSceneModelByHesh(uint64_t heshCode, ModelAsset* modelAsset);
private:
	AssimpConverter* assimp;
	std::unordered_map<std::string, ModelAsset*> modelAssets;
	ID3D11Device* device; 
	uint64_t FNV1a(const std::string& str);
};