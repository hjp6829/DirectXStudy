#include "RendererManager.h"
#include "EventBus.h"
#include "MeshRenderer.h"
#include "Object.h"

RendererManager::RendererManager()
{
	RegisterEventHandlers();
}
void RendererManager::RegisterEventHandlers()
{
	EventBus::Subscribe(EventType::ComponentAdded, [this](EventData* data) {SetNewMeshRenderComponent(data); });
}
void RendererManager::SetNewMeshRenderComponent(EventData* eventData)
{
	ComponentEventData* eventDataCast = static_cast<ComponentEventData*>(eventData);

	if (eventDataCast != nullptr)
	{
		MeshRenderer* meshComp = dynamic_cast<MeshRenderer*>(eventDataCast->component);
		if(meshComp == nullptr)
			return;
		meshComponenets[meshComp->GetOwnObject()->objectID] = meshComp;
	}
}
