#pragma once
#include "EventDefinitions.h"
#include <unordered_map>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

class MeshRenderer;
class PostProcessingManager;
class ObjectPickingManager;
class WorldManager;

class RendererManager
{
public:
	RendererManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11Texture2D* backBuffer);
	void Render(WorldManager* worldManager);

private:
	struct GlobalBuffer {
		XMFLOAT3 lightPos;
		float padding;
		XMFLOAT4 lightColor;
		XMFLOAT3 cameraPos;
		float padding2;
		float specularStrength;
		float shininess;
		float maxLightDistance;
		float padding3;
		XMFLOAT3 cameraForward;
		float padding4;
		XMFLOAT3 lightForward;
		float padding5;
	};
private:
	void RegisterEventHandlers();
	void SetNewMeshRenderComponent(EventData* eventData);
	std::unordered_map<uint64_t, MeshRenderer*> meshComponenets;
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // RGBA
	ComPtr<ID3D11RenderTargetView> pRenderTarget;
	ComPtr<ID3D11DepthStencilView> depthStencilView;
	ComPtr<ID3D11Buffer> lightConstantBuffer;
	PostProcessingManager* postProcessingManager;
	ObjectPickingManager* pickingManager;
	ID3D11DeviceContext* pContext;
	GlobalBuffer globalBuffer = {};
};