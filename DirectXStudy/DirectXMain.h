#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <cmath>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <functional>
#include "EventDefinitions.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

class WorldManager;
class RendererManager;

class DirectXMain {
	friend class Bindable;//친구 클래스는 private에 접근이 가능함
public:
	DirectXMain(HWND hWnd);
	void Start();
	void Render();
	void Shutdown();
	void EndDraw();
	ID3D11Device* GetDevice() { return pDevice.Get(); }
	ID3D11DeviceContext* GetContext() { return pContext.Get(); }
	void SetObjects(WorldManager* worldManager);
private:
	ComPtr<IDXGISwapChain> pSwap;
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pContext;

private:
	uint64_t currentOutlineID;
	RendererManager* rendererManager;
	WorldManager* worldManager;
	void RegisterEventHandlers();
	void SetSelectObjectID(EventData* eventData);
	void DeleteCurrentObject(EventData* eventData);
	void test(EventData* eventData);
	ActiveSceneMode testMode;
};