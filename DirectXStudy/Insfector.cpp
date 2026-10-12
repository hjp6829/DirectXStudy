#include "Insfector.h"
#include "imgui_impl_win32.h"
#include "ModelCreater.h"
#include "Object.h"
#include "Hierarchy.h"
#include "Mesh.h"
#include "Log.h"
#include "EventBus.h"
#include "EventDefinitions.h"

Insfector::Insfector()
{
	RegisterEventHandlers();
}

void Insfector::UpdateUI()
{
	if(currentObject == nullptr)
		return;
	currentObject->ShowInspectorUI();
}

void Insfector::ObjectDelete()
{
	currentObject=nullptr;
}
void Insfector::RegisterEventHandlers()
{
	EventBus::Subscribe(EventType::SelectObject, [this](EventData* data) {SetSelectObject(data); });
}
void Insfector::SetSelectObject(EventData* eventData)
{
	SelectObjectData* eventDataCast = dynamic_cast<SelectObjectData*>(eventData);

	if (eventDataCast != nullptr)
	{
		currentObject = eventDataCast->selectObject;
	}
}
