#include "WorldManager.h"
#include "EventBus.h"
#include "EventDefinitions.h"
#include "SceneManager.h"
#include "RuntimeManager.h"
#include "ScriptRegistry.h"
#include "ICamera.h"

WorldManager::WorldManager(ID3D11Device* device)
{
	RegisterEventHandlers();
	scriptRegistry = new ScriptRegistry();
	sceneManager = new SceneManager(device);
	runtimeManager = new RuntimeManager(device);
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
	currentManager->SetCreateObject(path);
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
		selectObjectData.selectObject = currentManager->GetObjectByID(eventDataCast->objectID);
		EventBus::Publish(EventType::SelectObject, &selectObjectData);
	}
}
void WorldManager::DeleteObject(EventData* eventData)
{
	SelectObjectIDData* eventDataCast = static_cast<SelectObjectIDData*>(eventData);

	if (eventDataCast != nullptr)
	{
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
