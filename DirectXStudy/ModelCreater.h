#pragma once
#include <vector>
#include <unordered_map>

class ModelNode;
class ModelAsset;
class SceneModel;
class AssimpConverter;

class ModelCreater
{
public:
	ModelCreater(ID3D11Device* device);
	ModelAsset* CreateModelAsset();
	void CreateChildSceneModel(ModelLoadData* modelLoadData, ModelNode* modelNode);
	SceneModel* CreateSceneModel(ModelAsset* modelAsset);
	void BuildSceneModelTree(ModelNode* modelNode, SceneModel* parentSceneModel);
private:
	AssimpConverter* assimp;
	ModelAsset* currentModelAsset;
	std::unordered_map<std::string, ModelAsset*> modelAssets;
	ID3D11Device* device;
};