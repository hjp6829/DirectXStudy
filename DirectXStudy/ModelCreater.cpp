#include "ModelCreater.h"
#include "DirectXMain.h"
#include "Mesh.h"
#include "Log.h"
#include "SceneModel.h"
#include "ModelAsset.h"
#include "ModelNode.h"
ModelCreater::ModelCreater(DirectXMain* dxdMain, AssimpConverter* assimp)
{
	dxd = dxdMain;
	this->assimp = assimp;
}

ModelAsset* ModelCreater::CreateModelAsset()
{
	std::vector<ModelLoadData*> modelDatas = assimp->GetModelData();
	currentModelAsset = new ModelAsset();
	currentModelAsset->currentNode = new ModelNode();
	CreateChildSceneModel(modelDatas[0]->childNodes[0], currentModelAsset->currentNode);
	return currentModelAsset;
}

void ModelCreater::CreateChildSceneModel(ModelLoadData* modelLoadData, ModelNode* modelNode)
{
	if (modelLoadData->childNodes.size() == 0)
	{
		for (int j = 0; j < modelLoadData->meshIDX.size(); j++)
		{
			asMesh* temp = assimp->GetMeshData(modelLoadData->meshIDX[j]);
			Mesh* mesh = new Mesh(dxd, modelLoadData, assimp, temp);
			modelNode->currentMeshs.push_back(mesh);
		}
		modelNode->modelName = modelLoadData->modelName;
		return;
	}
	for (int j = 0; j < modelLoadData->meshIDX.size(); j++)
	{
		asMesh* temp = assimp->GetMeshData(modelLoadData->meshIDX[j]);
		Mesh* mesh = new Mesh(dxd, modelLoadData, assimp, temp);
		modelNode->currentMeshs.push_back(mesh);
	}
	modelNode->modelName = modelLoadData->modelName;
	for (int i = 0; i < modelLoadData->childNodes.size(); i++)
	{
		ModelNode* childModelNode = new ModelNode();
		modelNode->childNodes.push_back(childModelNode);
		CreateChildSceneModel(modelLoadData->childNodes[i], childModelNode);
	}
}