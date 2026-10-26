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

WorldManager::WorldManager(ID3D11Device* device)
{
	RegisterEventHandlers();
	scriptRegistry = new ScriptRegistry();
	sceneManager = new SceneManager(device);
	runtimeManager = new RuntimeManager(device);
	modelCreater = new ModelCreater(device);
	currentManager = sceneManager;
}

void WorldManager::ObjectsUpdate(float deltaTime)
{
	currentManager->ObjectsUpdate(deltaTime);
}
void WorldManager::ObjectsLateUpdate(float deltaTime)
{
	currentManager->ObjectsLateUpdate(deltaTime);
}
void WorldManager::SetCreateObject(std::string path)
{
	ModelAsset* modelAsset = modelCreater->LoadModelFromFile(path);
	Object* objectTemp = modelCreater->CreateSceneObjectsFromModel(modelAsset, scriptRegistry,currentManager);
}
ICamera* WorldManager::GetCamera(int idx)
{
	if(activeSceneMode == ActiveSceneMode::SceneMode)
		return currentManager->GetSceneCamera();
	else
		return currentManager->GetCamera(idx);
}
void WorldManager::CreateObject(ObjectType type)
{
	Object* tempObject = currentManager->CreateObject(type);
	switch (type)
	{
	case ObjectType::Camera:
		{
			tempObject->objectName = "Camera" + std::to_string(++currentManager->cameraID);
			scriptRegistry->CreateScript("CameraComponent", tempObject);
			ICamera* cameraComponent = nullptr;
			for (auto& value : *tempObject->GetComponents())
			{
				cameraComponent = dynamic_cast<ICamera*>(value.get());
			}
			currentManager->SetCameraVector(cameraComponent);
			currentManager->SetNewObject(tempObject);
		}
		break;
	case ObjectType::Light:
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
void SceneManager::LoadSaveSceneFile()
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
		int objectType;
		item.at("ObjectType").get_to(objectType);
		Object* object = nullptr;
		if (objectType == 1)
			object = CreateSceneCamera(item);
		else
			object = CreateSceneObject(item, objectType, path);

		loadSceneObjects.insert({ object->objectID, object });
		if (item.at("isRootObject") == 1)
			objects[object->objectID] = object;
		allObjects[object->objectID] = object;
	}

	for (auto& [hashCode, objectTemp] : loadSceneObjects)
	{
		if (objectTemp->IsRootObject())
			continue;
		Object* parentObject = loadSceneObjects.at(objectTemp->parentobjectID);
		parentObject->InsertChildSceneObject(objectTemp);
	}
	isNoSaveFile = false;
	loadSceneObjects.clear();
}