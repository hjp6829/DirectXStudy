#include "WorldManager.h"
#include "EventBus.h"
#include "EventDefinitions.h"
#include "SceneManager.h"
#include "RuntimeManager.h"
#include "ScriptRegistry.h"
#include "ICamera.h"
#include "ModelCreater.h"
#include "Object.h"
#include "ModelAsset.h"
#include "MeshRenderer.h"
#include "ModelNode.h"
#include <fstream>
#include <unordered_map>
#include "LightComponent.h"
#include "SceneCameraComponent.h"
#include "CameraComponent.h"

WorldManager::WorldManager(ID3D11Device* device)
{
	RegisterEventHandlers();
	scriptRegistry = new ScriptRegistry();
	sceneManager = new SceneManager(device);
	runtimeManager = new RuntimeManager(device);
	modelCreater = new ModelCreater(device);
	currentManager = sceneManager;

	sceneManager->OnSaveScene = [this]() {//이벤트 버스
		SaveScene();
	};

	LoadSaveSceneFile();
}

void WorldManager::ObjectsUpdate(float deltaTime)
{
	currentManager->ObjectsUpdate(deltaTime);
}
void WorldManager::ObjectsLateUpdate(float deltaTime)
{
	currentManager->ObjectsLateUpdate(deltaTime);
}
ICamera* WorldManager::GetCamera(int idx)
{
	return currentManager->GetCamera(idx);
}
void WorldManager::CreateObject(std::string path)
{
	ModelAsset* modelAsset = modelCreater->LoadModelFromFile(path);
	Object* tempObject = currentManager->CreateObject();
	BuildSceneObjectTree(modelAsset->currentNode, tempObject);
}
void WorldManager::CreateObject(ObjectType type)
{
	Object* tempObject = currentManager->CreateObject();
	switch (type)
	{
	case ObjectType::Camera:
		{
			tempObject->objectName = "Camera" + std::to_string(++currentManager->cameraID);
			scriptRegistry->CreateScript("CameraComponent", tempObject);
			ICamera* cameraComponent = nullptr;
			for (auto& [key, value] : *tempObject->GetComponents())
			{
				cameraComponent = dynamic_cast<ICamera*>(value.get());
			}
			currentManager->SetCameraVector(cameraComponent);
			currentManager->SetNewObject(tempObject);
		}
		break;
	case ObjectType::Light:
		{
			tempObject->objectName = "MainLight";
			Component* lightTemp = scriptRegistry->CreateScript("LightComponent", tempObject);
			LightComponent* lightComp = static_cast<LightComponent*>(lightTemp);
			currentManager->SetLight(lightComp);
		}
		break;
	default:
		break;
	}
}
void WorldManager::RegisterEventHandlers()
{
	EventBus::Subscribe(EventType::ActiveSceneChanged, [this](EventData* data) {SetActiveMode(data); });
	EventBus::Subscribe(EventType::SelectObjectID, [this](EventData* data) {SetSelectObjectID(data); });
	EventBus::Subscribe(EventType::ObjectWillBeDeleted, [this](EventData* data) {DeleteObject(data); });
	EventBus::Subscribe(EventType::ComponentAdded, [this](EventData* data) {SetNewCameraComponent(data); });
}
void WorldManager::SetActiveMode(EventData* eventData)
{
	ActiveSceneChangedEventData* eventDataCast = static_cast<ActiveSceneChangedEventData*>(eventData);

	if (eventDataCast != nullptr)
	{
		if (eventDataCast->activeSceneMode == ActiveSceneMode::SceneMode)
		{
			runtimeManager->EndRunTime();
			currentManager = sceneManager;
		}
		else
		{
			runtimeManager->StartRuntime(sceneManager->GetAllObjects());
			currentManager = runtimeManager;
		}
		activeSceneMode = eventDataCast->activeSceneMode;
	}
}
void WorldManager::SetSelectObjectID(EventData* eventData)
{
	SelectObjectIDData* eventDataCast = static_cast<SelectObjectIDData*>(eventData);

	if (eventDataCast != nullptr)
	{
		SelectObjectData selectObjectData;
		selectedObjectID = eventDataCast->objectID;
		selectObjectData.selectObject = currentManager->GetObjectByID(selectedObjectID);
		EventBus::Publish(EventType::SelectObject, &selectObjectData);
	}
}
void WorldManager::DeleteObject(EventData* eventData)
{
	SelectObjectIDData* eventDataCast = static_cast<SelectObjectIDData*>(eventData);

	if (eventDataCast != nullptr)
	{
		selectedObjectID = 0;
		Object* tempObject = currentManager->GetObjectByID(eventDataCast->objectID);

		if (tempObject->IsRootObject())
		{
			(*currentManager->GetObjects()).erase(tempObject->objectID);
		}
		(*currentManager->GetAllObjects()).erase(tempObject->objectID);
		tempObject->RemoveModelData();
		DeleteChiledModels(tempObject);
		tempObject->childObjects.clear();
		delete tempObject;
	}
}
void WorldManager::DeleteChiledModels(Object* object)
{
	for (int i = 0; i < object->childObjects.size(); i++)
	{
		(*currentManager->GetAllObjects()).erase(object->childObjects[i]->objectID);
		DeleteChiledModels(object->childObjects[i]);
		delete object->childObjects[i];
	}
	object->childObjects.clear();
}

void WorldManager::BuildSceneObjectTree(ModelNode* modelNode, Object* parentObject)
{
	//SetModelToObject(parentObject, modelNode);
	parentObject->objectName = modelNode->modelName;
	Component* temp = scriptRegistry->CreateScript("MeshRenderer", parentObject);
	MeshRenderer* meshComp = static_cast<MeshRenderer*>(temp);
	meshComp->SetModelNode(modelNode);

	parentObject->GetTransform()->SetImportLocalMatrix(modelNode->localMatrix);
	for (int i = 0; i < modelNode->childNodes.size(); i++)
	{
		Object* childObject = currentManager->CreateObject(parentObject);
		BuildSceneObjectTree(modelNode->childNodes[i], childObject);
	}
}
void WorldManager::CreateInitJsonFile()
{
	Object* camObject = currentManager->CreateObject();
	camObject->objectName = "SceneCamera";
	scriptRegistry->CreateScript("CameraComponent", camObject);
	scriptRegistry->CreateScript("SceneCameraComponent", camObject);
	Object* lightObject = currentManager->CreateObject();
	lightObject->objectName = "MainLight";
	scriptRegistry->CreateScript("LightComponent", lightObject);
	currentManager->SetNewObject(camObject);
	currentManager->SetNewObject(lightObject);
	SaveScene();
	currentManager->ClearAllObject();
}

void WorldManager::SaveScene()
{
	Log::PrintLog("Scene Save");
	std::filesystem::path savePath = std::filesystem::path("SaveScene") / "Scene.json";
	std::filesystem::create_directories(savePath.parent_path());
	std::ofstream file(savePath);

	nlohmann::json jsonfile = nlohmann::json::array();

	for (auto& [key, value] : *currentManager->GetAllObjects())
	{
		Object* object = value;
		SaveSceneObjectData(object, jsonfile);
	}
	file << jsonfile.dump(4);
	file.close();
}

void WorldManager::SaveSceneObjectData(Object* object, nlohmann::json& jsonfile)
{
	nlohmann::json objectJson;
	object->Serialize(objectJson);
	jsonfile.push_back(objectJson);
}
//void WorldManager::CreateSceneObjectFromJsonData(std::filesystem::path path, uint64_t modelHeshCode)
//{
//	if (modelAssets.find(path.string()) != modelAssets.end())
//	{
//		CreateSingleSceneObjectByHesh(modelHeshCode, modelAssets[path.string()]);
//	}
//	ModelLoadData* modelLoadData = assimp->ReadAssetFile(path);
//	ModelAsset* modelAssetTemp = CreateModelAsset(modelLoadData, path);
//	modelAssets.insert({ path.string(), modelAssetTemp });
//	CreateSingleSceneObjectByHesh(modelHeshCode, modelAssetTemp);
//}
//void WorldManager::CreateSingleSceneObjectByHesh(uint64_t heshCode, ModelAsset* modelAsset)
//{
//	SceneObject* object = new SceneObject();
//	ModelNode* modelNode = modelAsset->modelNodesDic.at(heshCode);
//	SetModelToObject(object, modelNode);
//	object->objectID = ++manager->objectID;
//	Component* temp = scriptRegistry->CreateScript("MeshRenderer", parentObject);
//	MeshRenderer* meshComp = static_cast<MeshRenderer*>(temp);
//	meshComp->SetModelNode(modelNode);
//	manager->SetNewObject(parentObject);
//}
void WorldManager::LoadSaveSceneFile()
{
	using json = nlohmann::json;
	std::filesystem::path path = std::filesystem::current_path();
	std::filesystem::path saveFilePath = path / "SaveScene" / "Scene.json";
	if (!std::filesystem::exists(saveFilePath))
	{
		Log::PrintLog("No SaveFile Create Init SaveFile");
		CreateInitJsonFile();
	}
	std::ifstream file(saveFilePath);
	if (!file.is_open())
	{
		Log::PrintLog("path errer");
		return;
	}
	if (file.peek() == std::ifstream::traits_type::eof())
	{
		Log::PrintLog("file is empty");
		return;
	}
	json sceneJson;
	file >> sceneJson;

	std::unordered_map<uint64_t, Object*>loadSceneObjects;
	for (const auto& item : sceneJson)
	{
		std::string componentName;
		Object* object = new Object();

		if (item.contains("Components") && item["Components"].is_array())
		{
			for (const auto& componentJson : item["Components"])
			{
				componentJson.at("ComponentName").get_to(componentName);
				scriptRegistry->CreateScript(componentName, object);
			}
		}
		object->Deserialize(item);

		for (auto& [key, value] : *object->GetComponents())
		{
			MeshRenderer* meshComp = dynamic_cast<MeshRenderer*>(value.get());
			if (meshComp != nullptr)
				meshComp->SetModelNode(modelCreater->GetModelNode(meshComp->GetModelNodeHesh(), meshComp->GetModelPath()));
		}

		currentManager->SetObjectID(object->objectID);
		loadSceneObjects.insert({ object->objectID, object });
		currentManager->SetNewObject(object);
	}

	for (auto& [hashCode, objectTemp] : loadSceneObjects)
	{
		if (objectTemp->IsRootObject())
			continue;
		Object* parentObject = loadSceneObjects.at(objectTemp->parentobjectID);
		parentObject->InsertChildSceneObject(objectTemp);
	}
	loadSceneObjects.clear();
}
void WorldManager::SetNewCameraComponent(EventData* eventData)
{
	ComponentEventData* eventDataCast = static_cast<ComponentEventData*>(eventData);

	if (eventDataCast != nullptr)
	{
		CameraComponent* camComp = dynamic_cast<CameraComponent*>(eventDataCast->component);
		if (camComp != nullptr)
		{
			currentManager->SetCameraVector(camComp);
			return;
		}
		LightComponent* lightComp = dynamic_cast<LightComponent*>(eventDataCast->component);
		if (lightComp != nullptr)
		{
			currentManager->SetLight(lightComp);
			return;
		}
	}
}