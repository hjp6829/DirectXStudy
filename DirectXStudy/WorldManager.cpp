#include "WorldManager.h"
#include "EventBus.h"
#include "EventDefinitions.h"
#include "SceneManager.h"
#include "RuntimeManager.h"

WorldManager::WorldManager(ID3D11Device* device)
{
	RegisterEventHandlers();
	sceneManager = new SceneManager(device);
	runtimeManager = new RuntimeManager(device);
	currentManager = sceneManager;
	sceneManager->GetCamera(0);
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
			currentManager = sceneManager;
		else
			currentManager = runtimeManager;
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
