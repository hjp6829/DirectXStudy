#pragma once
#include "ConstantBuffer.h"

template <typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
	public:
		using ConstantBuffer<T>::ConstantBuffer;
		using ConstantBuffer<T>::Update;
		void Bind(DirectXMain& dxdMain) override {
			this->GetContext(dxdMain)->VSSetConstantBuffers(0, 1, this->constantBuffer.GetAddressOf());
		}
};