#include "Insfector.h"
#include "imgui_impl_win32.h"
#include "ModelCreater.h"
#include "Object.h"
#include "Hierarchy.h"
#include "Mesh.h"
#include "Log.h"
#include "EventBus.h"
#include "EventDefinitions.h"
#include "ScriptRegistry.h"
#include "ScriptComponent.h"
#include <filesystem>
#include "TestScript.h"

Insfector::Insfector(std::unordered_map<uint64_t, Object*>* modelContainer, std::unordered_map<uint64_t, Object*>* runtimeObjects)
{
	this->sceneObjects = modelContainer;
	this->runtimeObjects = runtimeObjects;
	RegisterEventHandlers();
	scriptRegistry = new ScriptRegistry();
}

void Insfector::UpdateUI()
{
	if(currentObject == nullptr)
		return;
	if (ImGui::Begin("Insfector")) {
		currentObject->ShowComponentInspectorUI();
	}
	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("ScriptList");
	}
	if (ImGui::BeginPopup("ScriptList"))
	{
		if (ImGui::Selectable("TestScript"))
		{
			std::unique_ptr<ScriptComponent> testScript = scriptRegistry->CreateScript("TestScript");
			currentObject->SetComponent(std::move(testScript));
		}

		ImGui::EndPopup();
	}
	ImGui::End();
}
void Insfector::RegisterEventHandlers()
{
	EventBus::Subscribe(EventType::SelectObject, [this](EventData* data) {SetSelectObject(data); });
	EventBus::Subscribe(EventType::ObjectWillBeDeleted, [this](EventData* data) {DeleteObject(data); });
	EventBus::Subscribe(EventType::ActiveSceneChanged, [this](EventData* data) {ActiveModeChange(data); });
}
void Insfector::SetSelectObject(EventData* eventData)
{
	SelectObjectData* eventDataCast = static_cast<SelectObjectData*>(eventData);

	if (eventDataCast != nullptr)
	{
		currentObject = eventDataCast->selectObject;
	}
}
void Insfector::DeleteObject(EventData* eventData)
{
	currentObject = nullptr;
}

void Insfector::ActiveModeChange(EventData* eventData)
{
	ActiveSceneChangedEventData* eventDataCast = static_cast<ActiveSceneChangedEventData*>(eventData);

	if (eventDataCast != nullptr)
	{
		if(eventDataCast->activeSceneMode == ActiveSceneMode::RuntimeMode)
			currentObject = (*runtimeObjects)[currentObject->objectID];
	}
}
