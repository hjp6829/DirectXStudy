#pragma once
#include "ConstantBuffer.h"

template <typename T>
class PSContantBuffer : public ConstantBuffer<T> {
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(ID3D11DeviceContext* context) override {
		this->GetContext(dxdMain)->PSSetConstantBuffers(0, 1, this->constantBuffer.GetAddressOf());
	}
};


