#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include <unordered_map>

#pragma comment(lib, "d3d11.lib")

using namespace Microsoft::WRL;

class Object;
class ICamera;
class MeshRenderer;

class ObjectPickingManager
{
public:
	ObjectPickingManager(ComPtr<ID3D11Device> pDevice);
	bool PickingObject(ID3D11DeviceContext* context, std::unordered_map<uint64_t, MeshRenderer*> meshComponenets,ICamera* cam, ID3D11DepthStencilView* depthStencilView);
private:
	ComPtr<ID3D11RenderTargetView> pPickingRenderTarget;
	ComPtr<ID3D11Texture2D> pickingStagingTexture;
	ComPtr<ID3D11Texture2D> pPickingTexture = NULL;
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // RGBA
};