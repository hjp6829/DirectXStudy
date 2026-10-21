#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <cmath>
#include <DirectXMath.h>
#include <vector>
#include "CameraObject.h"
#include "LightObject.h"
#include <string>
#include <functional>
#include "EventDefinitions.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

class AssimpConverter;
class ModelCreater;
class Object;
class ModelAsset;
class ModelNode;
class PostProcessingManager;
class WorldManager;
class ObjectPickingManager;

class DirectXMain {
	friend class Bindable;//친구 클래스는 private에 접근이 가능함
public:
	DirectXMain(HWND hWnd);
	void Start();
	void Render();
	void Shutdown();
	void EndDraw();
	CameraObject* GetCamera() { return cam; }
	ID3D11Device* GetDevice() { return pDevice.Get(); }
	ID3D11DeviceContext* GetContext() { return pContext.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView(){return depthStencilView.Get(); }
	void SetObjects(WorldManager* worldManager);
private:
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pRenderTarget;
	ComPtr<ID3D11DepthStencilView> depthStencilView;
	ComPtr<ID3D11Buffer> lightConstantBuffer;
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
	std::unordered_map<uint64_t, Object*>* rootObjects;
	CameraObject* cam;
	LightObject* light;
	GlobalBuffer globalBuffer = {};
	uint64_t currentOutlineID;
	PostProcessingManager* postProcessingManager;
	ObjectPickingManager* pickingManager;
	WorldManager* worldManager;
	void RegisterEventHandlers();
	void SetSelectObjectID(EventData* eventData);
	void DeleteCurrentObject(EventData* eventData);
};