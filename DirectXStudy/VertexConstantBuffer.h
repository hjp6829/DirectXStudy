#pragma once
#include "ConstantBuffer.h"

template <typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
	public:
		using ConstantBuffer<T>::ConstantBuffer;
		using ConstantBuffer<T>::Update;
		void Bind(ID3D11DeviceContext* context) override {
			context->VSSetConstantBuffers(0, 1, this->constantBuffer.GetAddressOf());
		}
};