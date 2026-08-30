#pragma once
#include "Bindable.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class VertexBuffer : public Bindable {
public:
	template<class V>
	VertexBuffer(DirectXMain& dxdMain, const std::vector<V>& vertices) : stride(sizeof(V)){
		D3D11_BUFFER_DESC vDes = {};
		vDes.ByteWidth = UINT(sizeof(V) * vertices.size());
		vDes.Usage = D3D11_USAGE_DEFAULT;
		vDes.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vDes.CPUAccessFlags = 0;
		vDes.MiscFlags = 0;
		vDes.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vd = {};
		vd.pSysMem = vertices.data();

		GetDevice(dxdMain)->CreateBuffer(&vDes, &vd, &vertexBuffer);
	}
	void Bind(DirectXMain& dxdMain) override;
private:
	ComPtr<ID3D11Buffer> vertexBuffer;
	UINT stride;
};