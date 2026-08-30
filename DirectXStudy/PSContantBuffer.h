#pragma once
#include "ConstantBuffer.h"

template <typename T>
class PSContantBuffer : public ConstantBuffer<T> {
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(DirectXMain& dxdMain) override {
		this->GetContext(dxdMain)->PSSetConstantBuffers(0, 1, this->constantBuffer.GetAddressOf());
	}
};


