#include "DirectXMain.h"
#include "Object.h"
#include "ModelCreater.h"
#include "imgui_impl_dx11.h"
#include "AssimpConverter.h"
#include "SceneModel.h"
#include "ModelAsset.h"
#include "ModelNode.h"

DirectXMain::DirectXMain(HWND hWnd, std::vector<SceneModel*>& sceneModels) : models(sceneModels)
{
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

	pDevice->CreateRenderTargetView(backBuffer, NULL, &pRenderTarget);
	backBuffer->Release();

	ID3D11Texture2D* pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = 1280.0;
	descDepth.Height = 960.0;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = descDepth.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	pDevice->CreateDepthStencilView(pDepthStencil, &dsvDesc, depthStencilView.GetAddressOf());

	pContext->OMSetRenderTargets(1, pRenderTarget.GetAddressOf(), depthStencilView.Get());

	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = 1280.0f;
	viewport.Height = 960.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	pContext->RSSetViewports(1, &viewport);
}

void DirectXMain::Start()
{
	globalBuffer.lightColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	globalBuffer.lightPos = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	D3D11_BUFFER_DESC sDes = {};
	sDes.ByteWidth = sizeof(globalBuffer);
	sDes.Usage = D3D11_USAGE_DYNAMIC;
	sDes.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	sDes.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sDes.MiscFlags = 0;
	sDes.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = &globalBuffer;

	pDevice->CreateBuffer(&sDes, &sd, lightConstantBuffer.GetAddressOf());
}

void DirectXMain::Update(float deltaTime)
{
	totalTime += deltaTime;
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->UpdateModel();
	}
}

void DirectXMain::Render()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // RGBA
	pContext->ClearRenderTargetView(pRenderTarget.Get(), clearColor);
	pContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	globalBuffer.cameraPos = cam->GetCameraPos();
	globalBuffer.lightPos = light->GetLightPos();
	globalBuffer.lightColor = light->GetLightColor();
	globalBuffer.specularStrength = light->GetSpecularStrength();
	globalBuffer.shininess = light->GetShininess();
	globalBuffer.maxLightDistance = light->GetMaxLightDistance();

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	pContext->Map(lightConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &globalBuffer, sizeof(globalBuffer));
	pContext->Unmap(lightConstantBuffer.Get(), 0);
	pContext->PSSetConstantBuffers(1, 1, lightConstantBuffer.GetAddressOf());

	for (int i = 0; i < models.size(); i++)
	{
		models[i]->RenderModel(this);
	}
}

void DirectXMain::Shutdown()
{
}

void DirectXMain::EndDraw()
{
	pSwap->Present(1, 0);
}


