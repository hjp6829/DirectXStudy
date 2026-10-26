#include "RendererManager.h"
#include "EventBus.h"
#include "MeshRenderer.h"
#include "Object.h"
#include "ObjectPickingManager.h"
#include "PostProcessingManager.h"
#include "Input.h"
#include "WorldManager.h"
#include "ICamera.h"
#include "LightObject.h"

RendererManager::RendererManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,ID3D11Texture2D* backBuffer)
{
	RegisterEventHandlers();

	this->pContext = pContext;

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

	HRESULT hr = pDevice->CreateDepthStencilView(pDepthStencil, &dsvDesc, depthStencilView.GetAddressOf());

	pContext->OMSetRenderTargets(1, pRenderTarget.GetAddressOf(), depthStencilView.Get());

	postProcessingManager = new PostProcessingManager(pDevice, pContext);
	pickingManager = new ObjectPickingManager(pDevice);

	globalBuffer.lightColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//globalBuffer.lightPos = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

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
void RendererManager::RegisterEventHandlers()
{
	EventBus::Subscribe(EventType::ComponentAdded, [this](EventData* data) {SetNewMeshRenderComponent(data); });
}
void RendererManager::SetNewMeshRenderComponent(EventData* eventData)
{
	ComponentEventData* eventDataCast = static_cast<ComponentEventData*>(eventData);

	if (eventDataCast != nullptr)
	{
		MeshRenderer* meshComp = dynamic_cast<MeshRenderer*>(eventDataCast->component);
		if(meshComp == nullptr)
		{
			return;
		}
		meshComponenets[meshComp->GetOwnObject()->objectID] = meshComp;
	}
}

void RendererManager::Render(WorldManager* worldManager)
{
	ICamera* cam = worldManager->GetCamera(0);
	LightObject* light = worldManager->GetLight();
	pContext->ClearRenderTargetView(pRenderTarget.Get(), clearColor);
	pContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	globalBuffer.cameraPos = cam->GetCameraPosition();
	globalBuffer.lightPos = light->GetTransform()->GetObjectPosition();
	globalBuffer.lightColor = light->GetLightColor();
	globalBuffer.specularStrength = light->GetSpecularStrength();
	globalBuffer.shininess = light->GetShininess();
	globalBuffer.maxLightDistance = light->GetMaxLightDistance();
	globalBuffer.cameraForward = cam->GetCameraForward();
	globalBuffer.lightForward = light->GetTransform()->GetForwardVector3();

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	pContext->Map(lightConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, &globalBuffer, sizeof(globalBuffer));
	pContext->Unmap(lightConstantBuffer.Get(), 0);
	pContext->PSSetConstantBuffers(1, 1, lightConstantBuffer.GetAddressOf());
	if (Input::GetMouseButtonDown(0))
	{
		if (pickingManager->PickingObject(pContext, meshComponenets, cam,depthStencilView.Get()))
		{
			pContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			pContext->OMSetRenderTargets(1, pRenderTarget.GetAddressOf(), depthStencilView.Get());
		}
	}
	for (auto& [key, value] : meshComponenets)
	{
		value->RenderMesh(RenderPass::Main, pContext, cam->GetViewMatrix(), cam->GetProjectionMatrix());
	}
	uint64_t selectedObjectID = worldManager->GetSelectedObjectID();
	if (selectedObjectID == 0)
		return;
	if (meshComponenets.find(selectedObjectID) == meshComponenets.end())
		return;
	postProcessingManager->OutlineRender(meshComponenets[selectedObjectID], pContext, cam);//그려주기
	pContext->OMSetRenderTargets(1, pRenderTarget.GetAddressOf(), depthStencilView.Get());
	postProcessingManager->PostProcessingRender();//그린거 사용하기
}
