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
}

void WorldManager::ObjectsUpdate(float deltaTime)
{
	currentManager->ObjectsUpdate(deltaTime);
}
void WorldManager::ObjectsLateUpdate(float deltaTime)
{
	currentManager->ObjectsLateUpdate(deltaTime);
}
void WorldManager::RegisterEventHandlers()
{
	EventBus::Subscribe(EventType::SelectObject, [this](EventData* data) {SetActiveMode(data); });
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
		currentRootObjects = currentManager->GetObjects();
		currentAllObjects = currentManager->GetAllObjects();
	}
}
