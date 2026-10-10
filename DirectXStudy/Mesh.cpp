#include "Mesh.h"
#include <filesystem>
#include "AssimpConverter.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "VertexBuffer.h"
#include "PrimitiveTopology.h"
#include "VertexConstantBufferContainer.h"
#include "PSContantBuffer.h"
#include "IndexBuffer.h"
#include "VertexConstantBuffer.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"

Mesh::Mesh(ID3D11Device* device, ModelLoadData* modelLoadData, AssimpConverter* assimp, asMesh* meshData)
{
	this->AssimpNodeMatrix = modelLoadData->mat;
	materialIDX = meshData->materialIDX;
	asMaterial* material = assimp->GetMaterial(materialIDX);
	//SetScale(0.01f,0.01f,0.01f);
	ComPtr<ID3DBlob> vertexShaderBlob;
	D3DReadFileToBlob(L"VertexShader.cso", &vertexShaderBlob);
	auto vertexShaderTemp = std::make_unique<VertexShader>(device, vertexShaderBlob.Get());
	outlineBindable.push_back(vertexShaderTemp.get());
	bindable.push_back(std::move(vertexShaderTemp));


	auto meshTextureTemp = std::make_unique<Texture>(material->textureView, 0);
	meshTexture = meshTextureTemp.get();
	bindable.push_back(std::move(meshTextureTemp));

	auto outlineSampleTemp = std::make_unique<Sampler>(device, 0);
	outlineBindable.push_back(outlineSampleTemp.get());
	bindable.push_back(std::move(outlineSampleTemp));

	auto normalTextureTemp = std::make_unique<Texture>(material->normalView, 1);
	normalTexture = normalTextureTemp.get();
	bindable.push_back(std::move(normalTextureTemp));
	bindable.push_back(std::make_unique<Sampler>(device, 1));

	auto inputlayoutTemp = std::make_unique<InputLayout>(device, vertexShaderBlob.Get());
	outlineBindable.push_back(inputlayoutTemp.get());
	bindable.push_back(std::move(inputlayoutTemp));

	ComPtr<ID3DBlob> pixelShaderBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &pixelShaderBlob);
	bindable.push_back(std::make_unique<PixelShader>(device, pixelShaderBlob.Get()));

	ComPtr<ID3DBlob> outlineShaderBlob;
	D3DReadFileToBlob(L"OutlineShader.cso", &outlineShaderBlob);
	outlineShader = std::make_unique<PixelShader>(device, outlineShaderBlob.Get());

	auto vertexBufferTemp = std::make_unique<VertexBuffer>(device, meshData->vertexs);
	outlineBindable.push_back(vertexBufferTemp.get());
	bindable.push_back(std::move(vertexBufferTemp));

	std::unique_ptr<IndexBuffer> indexBuffer = std::make_unique<IndexBuffer>(device, meshData->Indexs);
	IndexCount = indexBuffer->GetIndexCount();

	outlineBindable.push_back(indexBuffer.get());
	bindable.push_back(std::move(indexBuffer));

	auto primitiveTopologyTemp = std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	outlineBindable.push_back(primitiveTopologyTemp.get());
	bindable.push_back(std::move(primitiveTopologyTemp));

	localMatrix = AssimpNodeMatrix *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
		XMMatrixTranslation(position.x, position.y, position.z);

	cb.color = color;
	bindable.push_back(std::make_unique<PSContantBuffer<PSBuffer>>(device, cb)); 
	std::unique_ptr vertexConstantBufferTemp = std::make_unique<VertexConstantBufferContainer<ConstantBufferData>>(device,sb, *this);
	vertexConstantBufferContainer = vertexConstantBufferTemp.get();
	outlineBindable.push_back(vertexConstantBufferTemp.get());
	bindable.push_back(std::move(vertexConstantBufferTemp));
}

void Mesh::Render(ID3D11DeviceContext* context, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (!enable)
		return;
	DirectX::XMStoreFloat4x4(&sb.worldMatrix, localMatrix * worldMatrix);
	vertexConstantBufferContainer->SetModelMatrix(viewMatrix, projectionMatrix);
	for (int i = 0; i < bindable.size(); i++)
	{
		bindable[i]->Bind(context);
	}
	context->DrawIndexed(IndexCount,0,0);
}

void Mesh::OutlineRender(ID3D11DeviceContext* context, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	DirectX::XMStoreFloat4x4(&sb.worldMatrix, localMatrix * worldMatrix);
	vertexConstantBufferContainer->SetModelMatrix(viewMatrix, projectionMatrix);
	for (int i = 0; i < outlineBindable.size(); i++)
	{
		outlineBindable[i]->Bind(context);
	}
	outlineShader->Bind(context);
	context->DrawIndexed(IndexCount, 0, 0);
}

void Mesh::SetMaterialIDX(asMaterial* material,int idx)
{
	materialIDX = idx;
	normalTexture->ChangeNewTextureView(material->textureView);
	normalTexture->ChangeNewTextureView(material->normalView);
}

XMFLOAT3 Mesh::GetLocalPos()
{
	DirectX::XMVECTOR positionVec = localMatrix.r[3];

	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, positionVec);
	return position;
}
