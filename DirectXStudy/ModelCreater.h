#pragma once
#include <vector>
#include <unordered_map>
#include <d3d11.h>
#include <string>

class ModelNode;
class ModelAsset;
class SceneModel;
class AssimpConverter;
class ModelLoadData;

class ModelCreater
{
public:
	ModelCreater(ID3D11Device* device);
	ModelAsset* CreateModelAsset(ModelLoadData* modelData);
	void CreateChildSceneModel(ModelLoadData* modelLoadData, ModelNode* modelNode);
	SceneModel* CreateSceneModel(ModelAsset* modelAsset);
	void BuildSceneModelTree(ModelNode* modelNode, SceneModel* parentSceneModel);
	SceneModel* LoadModelFromFile(std::string path);
private:
	AssimpConverter* assimp;
	std::unordered_map<std::string, ModelAsset*> modelAssets;
	ID3D11Device* device;
};