#pragma once
#include <memory>
#include "VertexConstantBuffer.h"
#include <DirectXMath.h>
#include "Datas.h"

template <typename T>
class VertexConstantBufferContainer : public Bindable
{
	public:
		VertexConstantBufferContainer(ID3D11Device* device, const T consts,Object& object);
		void Bind(ID3D11DeviceContext* context) override;
		void SetModelMatrix(DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix);
	private:
		std::unique_ptr<VertexConstantBuffer<T>> vertexConstantBuffer;
		Object* parentObject;
		DirectX::XMMATRIX ViewMatrix;
		DirectX::XMMATRIX ProjectionMatrix;
};

template<typename T>
inline VertexConstantBufferContainer<T>::VertexConstantBufferContainer(ID3D11Device* device, const T consts, Object& object)
{
	vertexConstantBuffer = std::make_unique<VertexConstantBuffer<T>>(device, consts);
	parentObject= &object;
}


template<typename T>
inline void VertexConstantBufferContainer<T>::Bind(ID3D11DeviceContext* context)
{
	ConstantBufferData* cb = parentObject->GetVertexConstantBuffer();
	DirectX::XMMATRIX worldMAtrix = DirectX::XMLoadFloat4x4(&cb->worldMatrix);
	DirectX::XMStoreFloat4x4(&cb->finalMatrix, XMMatrixTranspose(worldMAtrix * ViewMatrix * XMMatrixPerspectiveLH(1.0, 3.0 / 4.0, 0.5, 1000)));
	DirectX::XMStoreFloat4x4(&cb->worldMatrix, XMMatrixTranspose(worldMAtrix));
	vertexConstantBuffer->Update(context, *parentObject->GetVertexConstantBuffer());

	vertexConstantBuffer->Bind(context);
}

template<typename T>
inline void VertexConstantBufferContainer<T>::SetModelMatrix(DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix)
{
	this->ViewMatrix = ViewMatrix;
	this->ProjectionMatrix = ProjectionMatrix;
}
