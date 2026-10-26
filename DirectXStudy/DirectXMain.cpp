#include "DirectXMain.h"
#include "ModelCreater.h"
#include "imgui_impl_dx11.h"
#include "AssimpConverter.h"
#include "SceneObject.h"
#include "ModelAsset.h"
#include "ModelNode.h"
#include "PostProcessingManager.h"
#include "Log.h"
#include "EventBus.h"
#include "Input.h"
#include "WorldManager.h"
#include "ObjectPickingManager.h"
#include "ICamera.h"
#include "RendererManager.h"

DirectXMain::DirectXMain(HWND hWnd)
{
	RegisterEventHandlers();
	DXGI_SWAP_CHAIN_DESC sd = {};//스왑체인 설계도 설정
	sd.BufferDesc.Width = 1280;
	sd.BufferDesc.Height = 960;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;//msaa사용안하면 최소1
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//화면에 핸더링
	sd.BufferCount = 1;//백버퍼
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;//창모드 
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		NULL,
		&pContext);

	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());

	ID3D11Texture2D* backBuffer = NULL;

	HRESULT result = pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = 1280.0f;
	viewport.Height = 960.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	pContext->RSSetViewports(1, &viewport);

	rendererManager = new RendererManager(pDevice.Get(),pContext.Get(), backBuffer);
}

void DirectXMain::Start()
{

}

void DirectXMain::Render()
{
	rendererManager->Render(worldManager);
}

void DirectXMain::Shutdown()
{
}

void DirectXMain::EndDraw()
{
	HRESULT hr = pSwap->Present(1, 0);
	if (FAILED(hr))
	{
		HRESULT reason = pDevice->GetDeviceRemovedReason();

		Log::PrintLog("Present failed");
	}
}

void DirectXMain::SetObjects(WorldManager* worldManager)
{
	this->worldManager = worldManager;
}

void DirectXMain::RegisterEventHandlers()
{
	EventBus::Subscribe(EventType::SelectObjectID, [this](EventData* data) {SetSelectObjectID(data); });
	EventBus::Subscribe(EventType::ObjectWillBeDeleted, [this](EventData* data) {DeleteCurrentObject(data); });
	EventBus::Subscribe(EventType::ActiveSceneChanged, [this](EventData* data) {test(data); });
}
void DirectXMain::SetSelectObjectID(EventData* eventData)
{
	SelectObjectIDData* eventDataCast = static_cast<SelectObjectIDData*>(eventData);

	if (eventDataCast != nullptr)
	{
		currentOutlineID = eventDataCast->objectID;
	}
}
void DirectXMain::DeleteCurrentObject(EventData* eventData)
{
	currentOutlineID = -1;
}

void DirectXMain::test(EventData* eventData)
{
	ActiveSceneChangedEventData* eventDataCast = static_cast<ActiveSceneChangedEventData*>(eventData);

	if (eventDataCast != nullptr)
	{
		testMode = eventDataCast->activeSceneMode;
	}
}


