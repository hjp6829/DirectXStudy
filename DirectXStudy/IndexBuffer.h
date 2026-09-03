#pragma once
#include "Bindable.h"
#include <wrl.h>
#include <DirectXMath.h>

class IndexBuffer : public Bindable {
public:
	template<class V>
	IndexBuffer(ID3D11Device* device, const std::vector<V>& indexs) : indexCount(indexs.size()){
		D3D11_BUFFER_DESC iDes = {};
		iDes.ByteWidth = (UINT)(indexs.size() * sizeof(V));;
		iDes.Usage = D3D11_USAGE_DEFAULT;
		iDes.BindFlags = D3D11_BIND_INDEX_BUFFER;
		iDes.CPUAccessFlags = 0;
		iDes.MiscFlags = 0;
		iDes.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA id = {};
		id.pSysMem = indexs.data();

		device->CreateBuffer(&iDes, &id, indexBuffer.GetAddressOf());
	}
	void Bind(ID3D11DeviceContext* context) override;
	UINT GetIndexCount();
private:
	ComPtr<ID3D11Buffer> indexBuffer;
	UINT indexCount;
};


