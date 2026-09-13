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

ModelAsset* ModelCreater::CreateModelAsset(ModelLoadData* modelData, std::filesystem::path modelName)
{
	ModelAsset* currentModelAsset = new ModelAsset();
	currentModelAsset->currentNode = new ModelNode();
	currentModelAsset->currentNode->modelHeshCode = FNV1a(modelName.stem().string());
	std::filesystem::path modelFilePath = modelName.parent_path().filename() / modelName.filename();
	currentModelAsset->currentNode->sourceModelPath = currentModelAsset->currentNode->modelName;
	currentModelAsset->modelNodesDic.insert({ currentModelAsset->currentNode->modelHeshCode, currentModelAsset->currentNode });
	CreateChildModelNode(modelData->childNodes[0], currentModelAsset->currentNode, currentModelAsset);
	currentModelAsset->currentNode->modelLocalPos = XMFLOAT3(0,0,0);
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
		return;
	}
	for (int j = 0; j < modelLoadData->meshIDX.size(); j++)
	{
		asMesh* temp = assimp->GetMeshData(modelLoadData->meshIDX[j]);
		Mesh* mesh = new Mesh(device, modelLoadData, assimp, temp);
		parentModelNode->currentMeshs.push_back(mesh);
	}
	parentModelNode->modelName = modelLoadData->modelName;
	int modelChildSize = modelLoadData->childNodes.size();
	modelAsset->modelNodesDic.insert({ parentModelNode->modelHeshCode, parentModelNode });
	for (int i = 0; i < modelChildSize; i++)
	{
		ModelNode* childModelNode = new ModelNode();
		parentModelNode->childNodes.push_back(childModelNode);
		childModelNode->sourceModelPath = parentModelNode->sourceModelPath;
		childModelNode->modelHeshCode = parentModelNode->modelHeshCode++;
		modelAsset->modelNodesDic.insert({childModelNode->modelHeshCode, childModelNode});
		CreateChildModelNode(modelLoadData->childNodes[i], childModelNode, modelAsset);
	}
	parentModelNode->modelLocalPos = modelLoadData->localPos;
	parentModelNode->modelLocalRot = modelLoadData->localRot;
	parentModelNode->modelLocalScale = modelLoadData->localScale;
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
		parentSceneModel->modelNamePath = modelNode->sourceModelPath;
	}
	else
	{
		parentSceneModel->currentModelNode = modelNode;
		parentSceneModel->modelName = modelNode->modelName;
		parentSceneModel->importedLocalPosition = modelNode->modelLocalPos;
		parentSceneModel->importedLocalRotation = modelNode->modelLocalRot;
		parentSceneModel->importedLocalScale = modelNode->modelLocalScale;
		parentSceneModel->modelNamePath = modelNode->sourceModelPath;
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
	if(modelAssets.find(path) != modelAssets.end())
	{
		return CreateSceneModel(modelAssets[path]);
	}
	ModelLoadData* modelLoadData = assimp->ReadAssetFile(path);
	ModelAsset* modelAssetTemp = CreateModelAsset(modelLoadData, pathtemp);
	modelAssets.insert({path, modelAssetTemp });
	return CreateSceneModel(modelAssetTemp);
}
SceneModel* ModelCreater::CreateSceneModelFromJsonData(std::filesystem::path path, JsonSceneModelData& jsonModelData)
{
	if (modelAssets.find(path.string()) != modelAssets.end())
	{
		return CreateSingleSceneModelByHesh(jsonModelData.modelHeshCode, modelAssets[path.string()]);
	}
	ModelLoadData* modelLoadData = assimp->ReadAssetFile(path);
	ModelAsset* modelAssetTemp = CreateModelAsset(modelLoadData, path.filename());
	modelAssets.insert({ path.string(), modelAssetTemp});
	return CreateSingleSceneModelByHesh(jsonModelData.modelHeshCode, modelAssetTemp);
}
SceneModel* ModelCreater::CreateSingleSceneModelByHesh(uint64_t heshCode, ModelAsset* modelAsset)
{
	ModelNode* modelNode = modelAsset->modelNodesDic[heshCode];
	SceneModel* sceneMode = new SceneModel();
	sceneMode->currentModelNode = modelNode;
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
