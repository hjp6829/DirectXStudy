#pragma once
#include "AssimpConverter.h"
#include <vector>

class DirectXMain;
class ModelNode;
class ModelAsset;

class ModelCreater
{
public:
	ModelCreater(DirectXMain* dxdMain, AssimpConverter* assimp);
	ModelAsset* CreateModelAsset();
	void CreateChildSceneModel(ModelLoadData* modelLoadData, ModelNode* modelNode);
private:
	DirectXMain* dxd;
	AssimpConverter* assimp;
	ModelAsset* currentModelAsset;
};