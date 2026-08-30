#pragma once
#include <memory>
#include "VertexConstantBuffer.h"
#include <DirectXMath.h>
#include "Datas.h"

template <typename T>
class VertexConstantBufferContainer : public Bindable
{
	public:
		VertexConstantBufferContainer(DirectXMain& dxdMain, const T consts,Object& object);
		void Bind(DirectXMain& dxdMain) override;
	private:
		std::unique_ptr<VertexConstantBuffer<T>> vertexConstantBuffer;
		Object* parentObject;
};

template<typename T>
inline VertexConstantBufferContainer<T>::VertexConstantBufferContainer(DirectXMain& dxdMain, const T consts, Object& object)
{
	vertexConstantBuffer = std::make_unique<VertexConstantBuffer<T>>(dxdMain, consts);
	parentObject= &object;
}


template<typename T>
inline void VertexConstantBufferContainer<T>::Bind(DirectXMain& dxdMain)
{
	DirectX::XMMATRIX ViewMatrix = dxdMain.GetCamera()->GetViewMatrix();
	ConstantBufferData* cb = parentObject->GetVertexConstantBuffer();
	DirectX::XMMATRIX worldMAtrix = DirectX::XMLoadFloat4x4(&cb->worldMatrix);
	DirectX::XMStoreFloat4x4(&cb->finalMatrix, XMMatrixTranspose(worldMAtrix * ViewMatrix * XMMatrixPerspectiveLH(1.0, 3.0 / 4.0, 0.5, 1000)));
	DirectX::XMStoreFloat4x4(&cb->worldMatrix, XMMatrixTranspose(worldMAtrix));
	vertexConstantBuffer->Update(dxdMain, *parentObject->GetVertexConstantBuffer());

	vertexConstantBuffer->Bind(dxdMain);
}
