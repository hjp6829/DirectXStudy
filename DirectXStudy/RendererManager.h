#pragma once
#include "EventDefinitions.h"
#include <unordered_map>
#include <wrl.h>

using namespace Microsoft::WRL;

class MeshRenderer;
class PostProcessingManager;
class ObjectPickingManager;

class RendererManager
{
public:
	RendererManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11Texture2D* backBuffer);
	void Render();
private:
	void RegisterEventHandlers();
	void SetNewMeshRenderComponent(EventData* eventData);
	std::unordered_map<uint64_t, MeshRenderer*> meshComponenets;
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // RGBA
	ComPtr<ID3D11RenderTargetView> pRenderTarget;
	ComPtr<ID3D11DepthStencilView> depthStencilView;
	PostProcessingManager* postProcessingManager;
	ObjectPickingManager* pickingManager;
	ID3D11DeviceContext* pContext;
};