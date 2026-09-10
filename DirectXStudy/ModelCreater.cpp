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

ModelAsset* ModelCreater::CreateModelAsset(ModelLoadData* modelData, std::string modelName)
{
	ModelAsset* currentModelAsset = new ModelAsset();
	currentModelAsset->currentNode = new ModelNode();
	currentModelAsset->currentNode->modelHeshCode = FNV1a(modelName);
	currentModelAsset->modelNodesDic.insert({ currentModelAsset->currentNode->modelHeshCode, currentModelAsset->currentNode });
	CreateChildModelNode(modelData->childNodes[0], currentModelAsset->currentNode, currentModelAsset);
	currentModelAsset->currentNode->modelLocalPos = XMFLOAT3(0,0,0);
	return currentModelAsset;
}

void ModelCreater::CreateChildModelNode(ModelLoadData* modelLoadData, ModelNode* modelNode, ModelAsset* modelAsset)
{
	if (modelLoadData->childNodes.size() == 0)
	{
		int meshSize = modelLoadData->meshIDX.size();
		for (int j = 0; j < meshSize; j++)
		{
			asMesh* temp = assimp->GetMeshData(modelLoadData->meshIDX[j]);
			Mesh* mesh = new Mesh(device, modelLoadData, assimp, temp);
			XMFLOAT3 localPosTemp = mesh->GetLocalPos();
			modelNode->currentMeshs.push_back(mesh);
		}
		modelNode->modelName = modelLoadData->modelName;
		modelNode->modelLocalPos = modelLoadData->localPos;
		modelNode->modelLocalRot = modelLoadData->localRot;
		modelNode->modelLocalScale = modelLoadData->localScale;
		return;
	}
	for (int j = 0; j < modelLoadData->meshIDX.size(); j++)
	{
		asMesh* temp = assimp->GetMeshData(modelLoadData->meshIDX[j]);
		Mesh* mesh = new Mesh(device, modelLoadData, assimp, temp);
		modelNode->currentMeshs.push_back(mesh);
	}
	modelNode->modelName = modelLoadData->modelName;
	int modelChildSize = modelLoadData->childNodes.size();
	for (int i = 0; i < modelChildSize; i++)
	{
		ModelNode* childModelNode = new ModelNode();
		modelNode->childNodes.push_back(childModelNode);
		childModelNode->modelHeshCode = modelNode->modelHeshCode++;
		modelAsset->modelNodesDic.insert({childModelNode->modelHeshCode, childModelNode});
		CreateChildModelNode(modelLoadData->childNodes[i], childModelNode, modelAsset);
	}
	modelNode->modelLocalPos = modelLoadData->localPos;
	modelNode->modelLocalRot = modelLoadData->localRot;
	modelNode->modelLocalScale = modelLoadData->localScale;
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
		parentSceneModel->importedLocalPosition = modelNode->modelLocalPos;
		parentSceneModel->importedLocalRotation = modelNode->modelLocalRot;
		parentSceneModel->importedLocalScale = modelNode->modelLocalScale;
	}
	else
	{
		parentSceneModel->currentModelNode = modelNode;
		parentSceneModel->modelName = modelNode->modelName;
		parentSceneModel->importedLocalPosition = modelNode->modelLocalPos;
		parentSceneModel->importedLocalRotation = modelNode->modelLocalRot;
		parentSceneModel->importedLocalScale = modelNode->modelLocalScale;
		for (int i = 0; i < modelNode->childNodes.size(); i++)
		{
			SceneModel* sceneModel = new SceneModel();
			sceneModel->currentModelNode = modelNode->childNodes[i];
			sceneModel->parentModel = parentSceneModel;
			parentSceneModel->childNodes.push_back(sceneModel);
			BuildSceneModelTree(modelNode->childNodes[i], sceneModel);
		}
	}
}

SceneModel* ModelCreater::LoadModelFromFile(std::string path)
{
	std::filesystem::path pathtemp = path;
	std::string modelName =	pathtemp.stem().string();
	if(modelAssets.find(path) != modelAssets.end())
	{
		return CreateSceneModel(modelAssets[path]);
	}
	ModelLoadData* modelLoadData = assimp->ReadAssetFile(path);
	ModelAsset* modelAssetTemp = CreateModelAsset(modelLoadData, modelName);
	modelAssets.insert({path, modelAssetTemp });
	return CreateSceneModel(modelAssetTemp);
}
uint64_t ModelCreater::FNV1a(const std::string& str)
{
	uint64_t hash = 14695981039346656037ULL;

	for (unsigned char c : str)
	{
		hash ^= c;
		hash *= 1099511628211ULL;
	}

	return hash;
}
