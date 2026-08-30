#include "ModelContainer.h"
#include "DirectXMain.h"
#include "Mesh.h"
#include "Log.h"
#include "SceneModel.h"
ModelContainer::ModelContainer(DirectXMain* dxdMain, AssimpConverter* assimp)
{
	dxd = dxdMain;
	this->assimp = assimp;
	std::vector<ModelLoadData*> modelDatas = assimp->GetModelData();
	currentSceneModel = new SceneModel();

	CreateSceneModel(modelDatas[0]->childNodes[0], currentSceneModel);
}

void ModelContainer::CreateSceneModel(ModelLoadData* modelLoadData, SceneModel* sceneModel)
{
	if (modelLoadData->childNodes.size() == 0)
	{
		for (int j = 0; j < modelLoadData->meshIDX.size(); j++)
		{
			asMesh* temp = assimp->GetMeshData(modelLoadData->meshIDX[j]);
			Mesh* mesh = new Mesh(dxd, modelLoadData, assimp, temp);
			sceneModel->currentMeshs.push_back(mesh);
		}
		sceneModel->modelName = modelLoadData->modelName;
		sceneModels.push_back(sceneModel);
		return;
	}
	for (int j = 0; j < modelLoadData->meshIDX.size(); j++)
	{
		asMesh* temp = assimp->GetMeshData(modelLoadData->meshIDX[j]);
		Mesh* mesh = new Mesh(dxd, modelLoadData, assimp, temp);
		sceneModel->currentMeshs.push_back(mesh);
	}
	sceneModel->modelName = modelLoadData->modelName;
	sceneModels.push_back(sceneModel);
	for (int i = 0; i < modelLoadData->childNodes.size(); i++)
	{
		SceneModel* childSceneModel = new SceneModel();
		sceneModel->childNodes.push_back(childSceneModel);
		CreateSceneModel(modelLoadData->childNodes[i], childSceneModel);
	}
}

void ModelContainer::RenderObjests()
{
	for (int i = 0; i < sceneModels.size(); i++)
	{
		sceneModels[i]->RenderMeshs(dxd);
	}
}

void ModelContainer::Update()
{
	for (int i = 0; i < sceneModels.size(); i++)
	{
		sceneModels[i]->UpdateMeshs();
	}
}
