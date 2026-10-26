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
		parentModelNode->localMatrix = modelLoadData->localMatrix;
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
	parentModelNode->localMatrix = modelLoadData->localMatrix;
}


Object* ModelCreater::CreateSceneObjectsFromModel(ModelAsset* modelAsset, ScriptRegistry* scriptRegistry,ObjectManager* manager)
{
	SceneObject* object = new SceneObject();
	object->SetRootNodeCheck(true);
	BuildSceneObjectTree(modelAsset->currentNode, object, scriptRegistry, manager);
	return object;
}

void ModelCreater::BuildSceneObjectTree(ModelNode* modelNode, Object* parentObject, ScriptRegistry* scriptRegistry, ObjectManager* manager)
{
	SetModelToObject(parentObject, modelNode);
	parentObject->objectID = ++manager->objectID;
	Component* temp = scriptRegistry->CreateScript("MeshRenderer", parentObject);
	MeshRenderer* meshComp = static_cast<MeshRenderer*>(temp);
	meshComp->SetModelNode(modelNode);
	manager->SetNewObject(parentObject);

	for (int i = 0; i < modelNode->childNodes.size(); i++)
	{
		SceneObject* object = new SceneObject();
		object->parentObject = parentObject;
		parentObject->childObjects.push_back(object);
		BuildSceneObjectTree(modelNode->childNodes[i], object, scriptRegistry, manager);
	}
}

ModelAsset* ModelCreater::LoadModelFromFile(std::string path)
{
	auto it = modelAssets.find(path);
	if(it != modelAssets.end())
	{
		return it->second;
	}
	ModelLoadData* modelLoadData = assimp->ReadAssetFile(path);
	ModelAsset* modelAssetTemp = CreateModelAsset(modelLoadData, path);
	modelAssets.insert({path, modelAssetTemp });
	return modelAssetTemp;
}
void ModelCreater::CreateSceneObjectFromJsonData(std::filesystem::path path, uint64_t modelHeshCode)
{
	if (modelAssets.find(path.string()) != modelAssets.end())
	{
		CreateSingleSceneObjectByHesh(modelHeshCode, modelAssets[path.string()]);
	}
	ModelLoadData* modelLoadData = assimp->ReadAssetFile(path);
	ModelAsset* modelAssetTemp = CreateModelAsset(modelLoadData, path);
	modelAssets.insert({ path.string(), modelAssetTemp});
	CreateSingleSceneObjectByHesh(modelHeshCode, modelAssetTemp);
}
void ModelCreater::CreateSingleSceneObjectByHesh(uint64_t heshCode, ModelAsset* modelAsset)
{
	SceneObject* object = new SceneObject();
	ModelNode* modelNode = modelAsset->modelNodesDic.at(heshCode);
	SetModelToObject(object, modelNode);
	object->objectID = ++manager->objectID;
	Component* temp = scriptRegistry->CreateScript("MeshRenderer", parentObject);
	MeshRenderer* meshComp = static_cast<MeshRenderer*>(temp);
	meshComp->SetModelNode(modelNode);
	manager->SetNewObject(parentObject);
}
void ModelCreater::SetModelToObject(Object* object, ModelNode* modelNode)
{
	object->objectName = modelNode->modelName;
	object->GetTransform()->SetImportLocalMatrix(modelNode->localMatrix);
	object->modelNamePath = modelNode->sourceModelPath;
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
