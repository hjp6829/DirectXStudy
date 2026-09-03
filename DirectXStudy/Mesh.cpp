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

Mesh::Mesh(ID3D11Device* device, ModelLoadData* modelLoadData, AssimpConverter* assimp, asMesh* meshData) : Object(device, modelLoadData, assimp, meshData)
{
	this->AssimpNodeMatrix = modelLoadData->mat;
	materialIDX = meshData->materialIDX;
	asMaterial* material = assimp->GetMaterial(materialIDX);
	//SetScale(0.01f,0.01f,0.01f);
	ComPtr<ID3DBlob> vertexShaderBlob;
	D3DReadFileToBlob(L"VertexShader.cso", &vertexShaderBlob);
	bindable.push_back(std::make_unique<VertexShader>(device, vertexShaderBlob.Get()));

	auto meshTextureTemp = std::make_unique<Texture>(material->textureView, 0);
	meshTexture = meshTextureTemp.get();
	bindable.push_back(std::move(meshTextureTemp));
	bindable.push_back(std::make_unique<Sampler>(device, 0));

	auto normalTextureTemp = std::make_unique<Texture>(material->normalView, 1);
	normalTexture = normalTextureTemp.get();
	bindable.push_back(std::move(normalTextureTemp));
	bindable.push_back(std::make_unique<Sampler>(device, 1));

	bindable.push_back(std::make_unique<InputLayout>(device, vertexShaderBlob.Get()));

	ComPtr<ID3DBlob> pixelShaderBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &pixelShaderBlob);
	bindable.push_back(std::make_unique<PixelShader>(device, pixelShaderBlob.Get()));

	bindable.push_back(std::make_unique<VertexBuffer>(device, meshData->vertexs));

	std::unique_ptr<IndexBuffer> indexBuffer = std::make_unique<IndexBuffer>(device, meshData->Indexs);
	IndexCount = indexBuffer->GetIndexCount();

	bindable.push_back(std::move(indexBuffer));

	bindable.push_back(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	localMatrix = AssimpNodeMatrix *
		XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
		XMMatrixTranslation(position.x, position.y, position.z);

	cb.color = color;
	bindable.push_back(std::make_unique<PSContantBuffer<PSBuffer>>(device, cb)); 
	std::unique_ptr vertexBufferTemp = std::make_unique<VertexConstantBufferContainer<ConstantBufferData>>(device,sb, *this);
	vertexConstantBufferContainer = vertexBufferTemp.get();
	bindable.push_back(std::move(vertexBufferTemp));
}

void Mesh::Update()
{
	if(!enable)
		return;

	Object::Update();
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

void Mesh::SetMaterialIDX(asMaterial* material,int idx)
{
	materialIDX = idx;
	normalTexture->ChangeNewTextureView(material->textureView);
	normalTexture->ChangeNewTextureView(material->normalView);
}
