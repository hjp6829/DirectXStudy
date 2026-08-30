#pragma once
#include "Bindable.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class IndexBuffer : public Bindable {
public:
	template<class V>
	IndexBuffer(DirectXMain& dxdMain, const std::vector<V>& indexs) : indexCount(indexs.size()){
		D3D11_BUFFER_DESC iDes = {};
		iDes.ByteWidth = (UINT)(indexs.size() * sizeof(V));;
		iDes.Usage = D3D11_USAGE_DEFAULT;
		iDes.BindFlags = D3D11_BIND_INDEX_BUFFER;
		iDes.CPUAccessFlags = 0;
		iDes.MiscFlags = 0;
		iDes.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA id = {};
		id.pSysMem = indexs.data();

		GetDevice(dxdMain)->CreateBuffer(&iDes, &id, &indexBuffer);
	}
	void Bind(DirectXMain& dxdMain) override;
	UINT GetIndexCount();
private:
	ComPtr<ID3D11Buffer> indexBuffer;
	UINT indexCount;
};


