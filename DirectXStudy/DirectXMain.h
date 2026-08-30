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
class ModelContainer;

class DirectXMain {
	friend class Bindable;//친구 클래스는 private에 접근이 가능함
	friend class AssimpConverter;
public:
	DirectXMain(HWND hWnd, AssimpConverter* assimp);
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
	AssimpConverter* GetAssimp(){return assimp; }
	void ReadModelByAssimp(std::wstring path);
	std::vector<ModelContainer*>& GetSceneModels(){return models;}
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
	std::vector<ModelContainer*> models;
	Mouse* currentMouse;
	Camera* cam;
	Light* light;
	GlobalBuffer globalBuffer = {};
	AssimpConverter* assimp;
};