#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <cmath>
#include <DirectXMath.h>
#include <vector>
#include "Mouse.h"
#include "CameraObject.h"
#include "LightObject.h"
#include <string>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

class AssimpConverter;
class ModelCreater;
class Object;
class ModelAsset;
class ModelNode;

class DirectXMain {
	friend class Bindable;//친구 클래스는 private에 접근이 가능함
public:
	DirectXMain(HWND hWnd);
	void Start();
	void Update(float deltaTime);
	void Render();
	void Shutdown();
	void EndDraw();
	void SetMouse(Mouse* mouse) { currentMouse = mouse; }
	CameraObject* GetCamera() { return cam; }
	void SetCamera(CameraObject*cam) { this->cam=cam;}
	void SetLight(LightObject* light) { this->light = light; }
	LightObject* GetLight() { return light; }
	ID3D11Device* GetDevice() { return pDevice.Get(); }
	ID3D11DeviceContext* GetContext() { return pContext.Get(); }
	void SetSceneObjects(std::vector<Object*>* objectVector){ objects = objectVector; }
private:
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pRenderTarget;
	ComPtr<ID3D11DepthStencilView> depthStencilView;

	ComPtr<ID3D11Buffer> lightConstantBuffer;
private:
	struct GlobalBuffer {
		XMFLOAT4 lightPos;
		XMFLOAT4 lightColor;
		XMFLOAT4 cameraPos;
		float specularStrength;
		float shininess;
		float maxLightDistance;
		float padding2;
	};
private:
	float totalTime;
	std::vector<Object*>* objects;
	Mouse* currentMouse;
	CameraObject* cam;
	LightObject* light;
	GlobalBuffer globalBuffer = {};
};