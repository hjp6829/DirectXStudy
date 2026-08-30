#pragma once
#include "AssimpConverter.h"
#include <vector>

class DirectXMain;
class SceneModel;

class ModelContainer
{
public:
	ModelContainer(DirectXMain* dxdMain, AssimpConverter* assimp);
	void CreateSceneModel(ModelLoadData* modelLoadData, SceneModel* sceneModel);
	void RenderObjests();
	void Update();
	SceneModel* GetSceneModel(){return currentSceneModel;}
private:
	std::vector<SceneModel*> sceneModels;
	DirectXMain* dxd;
	AssimpConverter* assimp;
	SceneModel* currentSceneModel;
};