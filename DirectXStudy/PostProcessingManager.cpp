#include "PostProcessingManager.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Object.h"
#include "DirectXMain.h"

PostProcessingManager::PostProcessingManager(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	device = pDevice;
	context = pContext;
	ID3D11Texture2D* pOutlineTexture = NULL;
	D3D11_TEXTURE2D_DESC descOutline;
	descOutline.Width = 1280.0;
	descOutline.Height = 960.0;
	descOutline.MipLevels = 1;
	descOutline.ArraySize = 1;
	descOutline.Format = DXGI_FORMAT_R8_UNORM;
	descOutline.SampleDesc.Count = 1;
	descOutline.SampleDesc.Quality = 0;
	descOutline.Usage = D3D11_USAGE_DEFAULT;
	descOutline.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	descOutline.CPUAccessFlags = 0;
	descOutline.MiscFlags = 0;

	pDevice->CreateTexture2D(&descOutline, nullptr, &pOutlineTexture);

	D3D11_RENDER_TARGET_VIEW_DESC outlineDesc = {};
	outlineDesc.Format = descOutline.Format;
	outlineDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	outlineDesc.Texture2D.MipSlice = 0;

	pDevice->CreateRenderTargetView(pOutlineTexture, &outlineDesc, pOutlineRenderTarget.GetAddressOf());
	pDevice->CreateShaderResourceView(pOutlineTexture, NULL, pPostProcessOutlineSRV.GetAddressOf());
	postProcessOutlineTexture = std::make_unique<Texture>(pPostProcessOutlineSRV, 0);

	ComPtr<ID3DBlob> vertexShaderBlob;
	D3DReadFileToBlob(L"PostProcessVertexShader.cso", &vertexShaderBlob);
	postProcessVertexShader = std::make_unique<VertexShader>(pDevice.Get(), vertexShaderBlob.Get());

	ComPtr<ID3DBlob> pixelShaderBlob;
	D3DReadFileToBlob(L"PostProcessPixelShader.cso", &pixelShaderBlob);
	postProcessPixelShader = std::make_unique<PixelShader>(pDevice.Get(), pixelShaderBlob.Get());

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D11_COLOR_WRITE_ENABLE_ALL;

	pDevice->CreateBlendState(&blendDesc, outlineBlendState.GetAddressOf());
}

void PostProcessingManager::PostProcessingRender()
{
	context->PSSetShaderResources(0, 1, pPostProcessOutlineSRV.GetAddressOf());//포스트 프로세싱에 아웃라인 랜더뷰의 텍스쳐를 넣어줌
	postProcessOutlineTexture->Bind(context.Get());
	postProcessVertexShader->Bind(context.Get());
	postProcessPixelShader->Bind(context.Get());
	context->IASetInputLayout(nullptr);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->OMSetBlendState(outlineBlendState.Get(), blendFactor, 0xffffffff);
	context->Draw(3, 0);
	context->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

void PostProcessingManager::OutlineRender(Object* currentObject,DirectXMain* dxdMain)
{
	context->ClearRenderTargetView(pOutlineRenderTarget.Get(), clearColor);
	context->OMSetRenderTargets(1, pOutlineRenderTarget.GetAddressOf(), nullptr);

	if (currentObject != nullptr)
	{
		currentObject->OutlineRenderObject(dxdMain);
	}
}
