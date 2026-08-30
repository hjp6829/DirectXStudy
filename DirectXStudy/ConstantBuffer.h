#pragma once
#include "Bindable.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace Microsoft::WRL;
using namespace DirectX;

//업데이트를 만들어서 거기서 constantBuffer값을 업데이트해줌
template <typename T>
class ConstantBuffer :public Bindable{
	public:
		void Update(DirectXMain& dxdMain, const T consts)
		{
			D3D11_MAPPED_SUBRESOURCE mapped = {};
			GetContext(dxdMain)->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
			memcpy(mapped.pData, &consts, sizeof(consts));
			GetContext(dxdMain)->Unmap(constantBuffer.Get(), 0);
		}
		ConstantBuffer(DirectXMain& dxdMain, const T consts) {
			D3D11_BUFFER_DESC sDes = {};
			sDes.ByteWidth = sizeof(consts);
			sDes.Usage = D3D11_USAGE_DYNAMIC;
			sDes.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			sDes.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			sDes.MiscFlags = 0;
			sDes.StructureByteStride = 0;
			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = &consts;

			GetDevice(dxdMain)->CreateBuffer(&sDes, &sd, constantBuffer.GetAddressOf());
		}
	protected:
		ComPtr<ID3D11Buffer> constantBuffer;
};