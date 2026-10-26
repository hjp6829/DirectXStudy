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
class ScriptRegistry;
class ObjectManager;

class ModelCreater
{
public:
	ModelCreater(ID3D11Device* device);
	ModelAsset* CreateModelAsset(ModelLoadData* modelData, std::filesystem::path modelName);
	void CreateChildModelNode(ModelLoadData* modelLoadData, ModelNode* modelNode, ModelAsset* modelAsset);
	
	Object* CreateSceneObjectsFromModel(ModelAsset* modelAsset, ScriptRegistry* scriptRegistry,  ObjectManager* manager);
	void BuildSceneObjectTree(ModelNode* modelNode, Object* parentObject, ScriptRegistry* scriptRegistry,  ObjectManager* manager);
	ModelAsset* LoadModelFromFile(std::string path);
	void CreateSceneObjectFromJsonData(std::filesystem::path path, uint64_t modelHeshCode);
	void CreateSingleSceneObjectByHesh(uint64_t heshCode, ModelAsset* modelAsset);
	void SetModelToObject(Object* object, ModelNode* modelNode);
private:
	AssimpConverter* assimp;
	std::unordered_map<std::string, ModelAsset*> modelAssets;
	ID3D11Device* device; 
	uint64_t FNV1a(const std::string& str);
};