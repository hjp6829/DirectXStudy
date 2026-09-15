#include "ModelCreater.h"
#include "DirectXMain.h"
#include "Mesh.h"
#include "Log.h"
#include "SceneObject.h"
#include "ModelAsset.h"
#include "ModelNode.h"
#include "AssimpConverter.h"
#include "Datas.h"

ModelCreater::ModelCreater(ID3D11Device* device)
{
	assimp = new AssimpConverter(device);
	this->device = device;
}

ModelAsset* ModelCreater::CreateModelAsset(ModelLoadData* modelData, std::filesystem::path modelName)
{
	ModelAsset* currentModelAsset = new ModelAsset();
	ModelNode* modelNodeTemp = new ModelNode();
	currentModelAsset->currentNode = modelNodeTemp;
	std::filesystem::path modelFilePath = modelName.parent_path().filename() / modelName.filename();
	modelNodeTemp->sourceModelPath = modelFilePath.string();
	CreateChildModelNode(modelData->childNodes[0], modelNodeTemp, currentModelAsset);
	return currentModelAsset;
}

void ModelCreater::CreateChildModelNode(ModelLoadData* modelLoadData, ModelNode* parentModelNode, ModelAsset* modelAsset)
{
	if (modelLoadData->childNodes.size() == 0)
	{
		int meshSize = modelLoadData->meshIDX.size();
		for (int j = 0; j < meshSize; j++)
		{
			asMesh* temp = assimp->GetMeshData(modelLoadData->meshIDX[j]);
			Mesh* mesh = new Mesh(device, modelLoadData, assimp, temp);
			XMFLOAT3 localPosTemp = mesh->GetLocalPos();
			parentModelNode->currentMeshs.push_back(mesh);
		}
		parentModelNode->modelName = modelLoadData->modelName;
		parentModelNode->modelLocalPos = modelLoadData->localPos;
		parentModelNode->modelLocalRot = modelLoadData->localRot;
		parentModelNode->modelLocalScale = modelLoadData->localScale;
		parentModelNode->modelHeshCode = FNV1a(parentModelNode->sourceModelPath + modelLoadData->modelName);
		modelAsset->modelNodesDic.insert({ parentModelNode->modelHeshCode, parentModelNode });
		return;
	}
	for (int j = 0; j < modelLoadData->meshIDX.size(); j++)
	{
		asMesh* temp = assimp->GetMeshData(modelLoadData->meshIDX[j]);
		Mesh* mesh = new Mesh(device, modelLoadData, assimp, temp);
		parentModelNode->currentMeshs.push_back(mesh);
	}
	parentModelNode->modelName = modelLoadData->modelName;
	parentModelNode->modelHeshCode = FNV1a(parentModelNode->sourceModelPath + modelLoadData->modelName);
	modelAsset->modelNodesDic.insert({ parentModelNode->modelHeshCode, parentModelNode });
	int modelChildSize = modelLoadData->childNodes.size();
	for (int i = 0; i < modelChildSize; i++)
	{
		ModelNode* childModelNode = new ModelNode();
		parentModelNode->childNodes.push_back(childModelNode);
		childModelNode->sourceModelPath = parentModelNode->sourceModelPath;
		CreateChildModelNode(modelLoadData->childNodes[i], childModelNode, modelAsset);
	}
	parentModelNode->modelLocalPos = modelLoadData->localPos;
	parentModelNode->modelLocalRot = modelLoadData->localRot;
	parentModelNode->modelLocalScale = modelLoadData->localScale;
}


SceneObject* ModelCreater::CreateSceneObject(ModelAsset* modelAsset)
{
	SceneObject* sceneObject = new SceneObject();
	BuildSceneObjectTree(modelAsset->currentNode, sceneObject);
	return sceneObject;
}

void ModelCreater::BuildSceneObjectTree(ModelNode* modelNode, SceneObject* parentSceneObject)
{
	if (modelNode->childNodes.size() == 0)
	{
		parentSceneObject->currentModelNode = modelNode;
		parentSceneObject->modelName = modelNode->modelName;
		parentSceneObject->SetLocalTransform(modelNode->modelLocalPos, modelNode->modelLocalRot, modelNode->modelLocalScale);
		parentSceneObject->modelNamePath = modelNode->sourceModelPath;
	}
	else
	{
		parentSceneObject->currentModelNode = modelNode;
		parentSceneObject->modelName = modelNode->modelName;
		parentSceneObject->SetLocalTransform(modelNode->modelLocalPos, modelNode->modelLocalRot, modelNode->modelLocalScale);
		parentSceneObject->modelNamePath = modelNode->sourceModelPath;
		for (int i = 0; i < modelNode->childNodes.size(); i++)
		{
			SceneObject* sceneObject = new SceneObject();
			sceneObject->currentModelNode = modelNode->childNodes[i];
			sceneObject->parentModel = parentSceneObject;
			parentSceneObject->childModels.push_back(sceneObject);
			BuildSceneObjectTree(modelNode->childNodes[i], sceneObject);
		}
	}
}

SceneObject* ModelCreater::LoadModelFromFile(std::string path)
{
	auto it = modelAssets.find(path);
	if(it != modelAssets.end())
	{
		return CreateSceneObject(it->second);
	}
	ModelLoadData* modelLoadData = assimp->ReadAssetFile(path);
	ModelAsset* modelAssetTemp = CreateModelAsset(modelLoadData, path);
	modelAssets.insert({path, modelAssetTemp });
	return CreateSceneObject(modelAssetTemp);
}
SceneObject* ModelCreater::CreateSceneObjectFromJsonData(std::filesystem::path path, JsonSceneObjectData& jsonModelData)
{
	if (modelAssets.find(path.string()) != modelAssets.end())
	{
		return CreateSingleSceneObjectByHesh(jsonModelData.modelHeshCode, modelAssets[path.string()]);
	}
	ModelLoadData* modelLoadData = assimp->ReadAssetFile(path);
	ModelAsset* modelAssetTemp = CreateModelAsset(modelLoadData, path);
	modelAssets.insert({ path.string(), modelAssetTemp});
	return CreateSingleSceneObjectByHesh(jsonModelData.modelHeshCode, modelAssetTemp);
}
SceneObject* ModelCreater::CreateSingleSceneObjectByHesh(uint64_t heshCode, ModelAsset* modelAsset)
{
	ModelNode* modelNode = modelAsset->modelNodesDic.at(heshCode);
	SceneObject* sceneMode = new SceneObject();
	sceneMode->currentModelNode = modelNode;
	sceneMode->currentModelNode = modelNode;
	sceneMode->modelName = modelNode->modelName;
	sceneMode->SetLocalTransform(modelNode->modelLocalPos, modelNode->modelLocalRot, modelNode->modelLocalScale);
	sceneMode->modelNamePath = modelNode->sourceModelPath;
	return sceneMode;
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
