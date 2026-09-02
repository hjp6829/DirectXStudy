#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <cmath>
#include <DirectXMath.h>
#include <vector>
#include "Mouse.h"
#include "Camera.h"
#include "Light.h"
#include <string>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

class Object;
class AssimpConverter;
class ModelCreater;
class SceneModel;
class ModelAsset;
class ModelNode;

class DirectXMain {
	friend class Bindable;//친구 클래스는 private에 접근이 가능함
public:
	DirectXMain(HWND hWnd,std::vector<SceneModel*>& sceneModels);
	void Start();
	void Update(float deltaTime);
	void Render();
	void Shutdown();
	void Draw(UINT indexCount);
	void EndDraw();
	void SetMouse(Mouse* mouse) { currentMouse = mouse; }
	Camera* GetCamera() { return cam; }
	void SetCamera(Camera *cam) { this->cam=cam;}
	void SetLight(Light* light) { this->light = light; }
	Light* GetLight() { return light; }
	ID3D11Device* GetDevice() { return pDevice.Get(); }
	ID3D11DeviceContext* GetContext() { return pContext.Get(); }
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
	std::vector<SceneModel*>& models;
	Mouse* currentMouse;
	Camera* cam;
	Light* light;
	GlobalBuffer globalBuffer = {};
};