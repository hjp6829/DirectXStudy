#pragma once
#include <vector>
#include <unordered_map>
#include <d3d11.h>
#include <string>
#include <filesystem>

class ModelNode;
class ModelAsset;
class Object;
class AssimpConverter;
class ModelLoadData;
struct JsonSceneObjectData;

class ModelCreater
{
public:
	ModelCreater(ID3D11Device* device);
	ModelAsset* CreateModelAsset(ModelLoadData* modelData, std::filesystem::path modelName);
	void CreateChildModelNode(ModelLoadData* modelLoadData, ModelNode* modelNode, ModelAsset* modelAsset);
	
	Object* CreateSceneObject(ModelAsset* modelAsset);
	void BuildSceneObjectTree(ModelNode* modelNode, Object* parentObject);
	Object* LoadModelFromFile(std::string path);
	Object* CreateSceneObjectFromJsonData(std::filesystem::path path, JsonSceneObjectData& jsonModelData);
	Object* CreateSingleSceneObjectByHesh(uint64_t heshCode, ModelAsset* modelAsset);
private:
	AssimpConverter* assimp;
	std::unordered_map<std::string, ModelAsset*> modelAssets;
	ID3D11Device* device; 
	uint64_t FNV1a(const std::string& str);
};