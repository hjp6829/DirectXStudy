#include "ModelCreater.h"
#include "DirectXMain.h"
#include "Mesh.h"
#include "Log.h"
#include "Object.h"
#include "ModelAsset.h"
#include "ModelNode.h"
#include "AssimpConverter.h"
#include "Datas.h"
#include "SceneObject.h"
#include "ScriptRegistry.h"
#include "Component.h"
#include "MeshRenderer.h"
#include "ObjectManager.h"

ModelCreater::ModelCreater(ID3D11Device* device)
{
	this->device = device;
}

ModelAsset* ModelCreater::CreateModelAsset(ModelLoadData* modelData, const MetaData* metaData)
{
	ModelAsset* currentModelAsset = new ModelAsset();
	ModelNode* modelNodeTemp = new ModelNode();
	modelNodeTemp->GUID = metaData->GUID;
	currentModelAsset->currentNode = modelNodeTemp;
	std::filesystem::path modelFilePath = metaData->assetPath.parent_path().filename() / metaData->assetPath.filename();
	CreateChildModelNode(modelData->childNodes[0], modelNodeTemp, currentModelAsset);
	return currentModelAsset;
}

void ModelCreater::CreateChildModelNode(ModelLoadData* modelLoadData, ModelNode* parentModelNode, ModelAsset* modelAsset)
{
	if (modelLoadData->childNodes.size() == 0)
	{
		int meshSize = modelLoadData->meshs.size();
		for (int j = 0; j < meshSize; j++)
		{
			asMesh* temp = modelLoadData->meshs[j];
			MeshData meshData;
			meshData.Indexs = temp->Indexs;
			meshData.vertexs = temp->vertexs;
			//meshData.material = assimp->GetMaterial(temp->materialIDX);
			Mesh* mesh = new Mesh(device, meshData);
			XMFLOAT3 localPosTemp = mesh->GetLocalPos();
			parentModelNode->currentMeshs.push_back(mesh);
		}
		parentModelNode->modelName = modelLoadData->modelName;
		parentModelNode->localMatrix = modelLoadData->localMatrix;
		parentModelNode->modelLocalHesh = FNV1a(modelLoadData->localPath);
		modelAsset->SetModelNode( parentModelNode->modelLocalHesh, parentModelNode );
		return;
	}
	for (int j = 0; j < modelLoadData->meshs.size(); j++)
	{
		asMesh* temp = modelLoadData->meshs[j];
		MeshData meshData;
		meshData.Indexs = temp->Indexs;
		meshData.vertexs = temp->vertexs;
		//meshData.material = assimp->GetMaterial(temp->materialIDX);
		Mesh* mesh = new Mesh(device, meshData);
		parentModelNode->currentMeshs.push_back(mesh);
	}
	parentModelNode->modelName = modelLoadData->modelName;
	parentModelNode->modelLocalHesh = FNV1a(modelLoadData->localPath);
	modelAsset->SetModelNode(parentModelNode->modelLocalHesh, parentModelNode);
	int modelChildSize = modelLoadData->childNodes.size();
	for (int i = 0; i < modelChildSize; i++)
	{
		ModelNode* childModelNode = new ModelNode();
		parentModelNode->childNodes.push_back(childModelNode);
		childModelNode->GUID = parentModelNode->GUID;
		CreateChildModelNode(modelLoadData->childNodes[i], childModelNode, modelAsset);
	}
	parentModelNode->localMatrix = modelLoadData->localMatrix;
}
ModelAsset* ModelCreater::LoadModelFromFile(const MetaData* metaData,ModelLoadData* modelLoadData)
{
	auto it = modelAssets.find(metaData->GUID);
	if(it != modelAssets.end())
	{
		return it->second;
	}
	ModelAsset* modelAssetTemp = CreateModelAsset(modelLoadData, metaData);
	modelAssets.insert({ metaData->GUID, modelAssetTemp });
	return modelAssetTemp;
}

ModelNode* ModelCreater::GetModelNode(const MetaData* metaData,uint64_t localHesh)
{
	ModelAsset* assetTemp = LoadModelFromFile(metaData);
	ModelNode* modelNodeTemp = assetTemp->GetModelNode(localHesh);
	if(modelNodeTemp != nullptr)
		return modelNodeTemp;

	return nullptr;
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
