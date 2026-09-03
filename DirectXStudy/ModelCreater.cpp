#include "ModelCreater.h"
#include "DirectXMain.h"
#include "Mesh.h"
#include "Log.h"
#include "SceneModel.h"
#include "ModelAsset.h"
#include "ModelNode.h"
#include "AssimpConverter.h"
#include "Datas.h"

ModelCreater::ModelCreater(ID3D11Device* device)
{
	assimp = new AssimpConverter(device);
	this->device = device;
}

ModelAsset* ModelCreater::CreateModelAsset(ModelLoadData* modelData)
{
	ModelAsset* currentModelAsset = new ModelAsset();
	currentModelAsset->currentNode = new ModelNode();
	CreateChildSceneModel(modelData->childNodes[0], currentModelAsset->currentNode);
	return currentModelAsset;
}

void ModelCreater::CreateChildSceneModel(ModelLoadData* modelLoadData, ModelNode* modelNode)
{
	if (modelLoadData->childNodes.size() == 0)
	{
		for (int j = 0; j < modelLoadData->meshIDX.size(); j++)
		{
			asMesh* temp = assimp->GetMeshData(modelLoadData->meshIDX[j]);
			Mesh* mesh = new Mesh(device, modelLoadData, assimp, temp);
			modelNode->currentMeshs.push_back(mesh);
		}
		modelNode->modelName = modelLoadData->modelName;
		return;
	}
	for (int j = 0; j < modelLoadData->meshIDX.size(); j++)
	{
		asMesh* temp = assimp->GetMeshData(modelLoadData->meshIDX[j]);
		Mesh* mesh = new Mesh(device, modelLoadData, assimp, temp);
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


SceneModel* ModelCreater::CreateSceneModel(ModelAsset* modelAsset)
{
	SceneModel* sceneModel = new SceneModel();
	BuildSceneModelTree(modelAsset->currentNode, sceneModel);
	return sceneModel;
}

void ModelCreater::BuildSceneModelTree(ModelNode* modelNode, SceneModel* parentSceneModel)
{
	if (modelNode->childNodes.size() == 0)
	{
		parentSceneModel->currentModelNode = modelNode;
		parentSceneModel->modelName = modelNode->modelName;
	}
	else
	{
		parentSceneModel->currentModelNode = modelNode;
		parentSceneModel->modelName = modelNode->modelName;
		for (int i = 0; i < modelNode->childNodes.size(); i++)
		{
			SceneModel* sceneModel = new SceneModel();
			sceneModel->currentModelNode = modelNode->childNodes[i];
			parentSceneModel->childNodes.push_back(sceneModel);
			BuildSceneModelTree(modelNode->childNodes[i], sceneModel);
		}
	}
}

SceneModel* ModelCreater::LoadModelFromFile(std::string path)
{
	if(modelAssets.find(path) != modelAssets.end())
	{
		return CreateSceneModel(modelAssets[path]);
	}
	ModelLoadData* modelLoadData = assimp->ReadAssetFile(path);
	ModelAsset* modelAssetTemp = CreateModelAsset(modelLoadData);
	modelAssets.insert({path, modelAssetTemp });
	return CreateSceneModel(modelAssetTemp);
}
