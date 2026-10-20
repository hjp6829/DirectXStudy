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
#include "WorldManager.h"

Insfector::Insfector(WorldManager* worldManager)
{
	this->worldManager = worldManager;
	RegisterEventHandlers();
	scriptRegistry = new ScriptRegistry();
}

void Insfector::UpdateUI()
{
	if(currentObjectID == -1)
		return;
	Object* currentObject = worldManager->GetObjectByID(currentObjectID);
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
	EventBus::Subscribe(EventType::SelectObjectID, [this](EventData* data) {SetSelectObjectID(data); });
	EventBus::Subscribe(EventType::ObjectWillBeDeleted, [this](EventData* data) {DeleteObject(data); });
}
void Insfector::SetSelectObjectID(EventData* eventData)
{
	SelectObjectIDData* eventDataCast = static_cast<SelectObjectIDData*>(eventData);

	if (eventDataCast != nullptr)
	{
		currentObjectID = eventDataCast->objectID;
	}
}
void Insfector::DeleteObject(EventData* eventData)
{
	currentObjectID = -1;
}

