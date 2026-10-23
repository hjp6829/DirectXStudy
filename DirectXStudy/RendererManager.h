#pragma once
#include "EventDefinitions.h"
#include <unordered_map>

class MeshRenderer;

class RendererManager
{
public:
	RendererManager();
	void Render();
private:
	void RegisterEventHandlers();
	void SetNewMeshRenderComponent(EventData* eventData);
	std::unordered_map<uint64_t, MeshRenderer*> meshComponenets;
};