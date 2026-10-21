#include "ObjectPickingManager.h"
#include "Input.h"
#include "imgui_impl_dx11.h"
#include <DirectXMath.h>
#include "EventDefinitions.h"
#include "EventBus.h"
#include "Object.h"
#include "DirectXMain.h"

ObjectPickingManager::ObjectPickingManager(ComPtr<ID3D11Device> pDevice)
{
	D3D11_TEXTURE2D_DESC descPicking;
	descPicking.Width = 1280.0;
	descPicking.Height = 960.0;
	descPicking.MipLevels = 1;
	descPicking.ArraySize = 1;
	descPicking.Format = DXGI_FORMAT_R32_UINT;
	descPicking.SampleDesc.Count = 1;
	descPicking.SampleDesc.Quality = 0;
	descPicking.Usage = D3D11_USAGE_DEFAULT;
	descPicking.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	descPicking.CPUAccessFlags = 0;
	descPicking.MiscFlags = 0;

	pDevice->CreateTexture2D(&descPicking, nullptr, &pPickingTexture);

	D3D11_RENDER_TARGET_VIEW_DESC outlineDesc = {};
	outlineDesc.Format = descPicking.Format;
	outlineDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	outlineDesc.Texture2D.MipSlice = 0;

	pDevice->CreateRenderTargetView(pPickingTexture.Get(), &outlineDesc, pPickingRenderTarget.GetAddressOf());

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = 1;
	desc.Height = 1;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_UINT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	pDevice->CreateTexture2D(&desc, nullptr, pickingStagingTexture.GetAddressOf());
}

bool ObjectPickingManager::PickingObject(DirectXMain* dxdMain, std::unordered_map<uint64_t, Object*>* rootObjects)
{
		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse)
		{
			dxdMain->GetContext()->ClearRenderTargetView(pPickingRenderTarget.Get(), clearColor);
			dxdMain->GetContext()->OMSetRenderTargets(1, pPickingRenderTarget.GetAddressOf(), dxdMain->GetDepthStencilView());

			for (auto& [key, value] : *rootObjects)
			{
				value->RenderObject(RenderPass::Picking, dxdMain);
			}
			POINT mousePos = Input::mousePosition();
			D3D11_BOX sourceBox = {};

			sourceBox.left = mousePos.x;
			sourceBox.right = mousePos.x + 1;

			sourceBox.top = mousePos.y;
			sourceBox.bottom = mousePos.y + 1;

			sourceBox.front = 0;
			sourceBox.back = 1;

			dxdMain->GetContext()->CopySubresourceRegion(
				pickingStagingTexture.Get(),
				0,
				0, 0, 0,
				pPickingTexture.Get(),
				0,
				&sourceBox
			);

			D3D11_MAPPED_SUBRESOURCE mapped = {};

			HRESULT hr = dxdMain->GetContext()->Map(
				pickingStagingTexture.Get(),
				0,
				D3D11_MAP_READ,
				0,
				&mapped
			);

			if (SUCCEEDED(hr))
			{
				uint64_t objectID =
					*static_cast<const uint64_t*>(mapped.pData);

				dxdMain->GetContext()->Unmap(pickingStagingTexture.Get(), 0);

				if (objectID != 0)
				{
					SelectObjectIDData selectObjectIDData;
					selectObjectIDData.objectID = objectID;
					EventBus::Publish(EventType::SelectObjectID, &selectObjectIDData);
				}
			}

			return true;
		}
		return false;
}
