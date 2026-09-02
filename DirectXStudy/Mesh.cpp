#include "Mesh.h"
#include <filesystem>
#include "AssimpConverter.h"

Mesh::Mesh(ID3D11Device* device, ModelLoadData* modelLoadData, AssimpConverter* assimp, asMesh* meshData) : Object(dxdMain, modelLoadData, assimp, meshData)
{
	this->dxdMain = dxdMain;
	this->AssimpNodeMatrix = modelLoadData->mat;
	materialIDX = meshData->materialIDX;
	asMaterial* material = assimp->GetMaterial(materialIDX);
	//SetScale(0.01f,0.01f,0.01f);
	ComPtr<ID3DBlob> vertexShaderBlob;
	D3DReadFileToBlob(L"VertexShader.cso", &vertexShaderBlob);
	bindable.push_back(std::make_unique<VertexShader>(device, vertexShaderBlob.Get()));

	auto meshTextureTemp = std::make_unique<Texture>(device, material->textureView, 0);
	meshTexture = meshTextureTemp.get();
	bindable.push_back(std::move(meshTextureTemp));
	bindable.push_back(std::make_unique<Sampler>(device, 0));

	auto normalTextureTemp = std::make_unique<Texture>(device, material->normalView, 1);
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
	bindable.push_back(std::make_unique<VertexConstantBufferContainer<ConstantBufferData>>(*dxdMain, sb, *this));
}

void Mesh::Update()
{
	if(!enable)
		return;

	Object::Update();
}

void Mesh::Render(ID3D11DeviceContext* context, XMMATRIX worldMatrix)
{
	if (!enable)
		return;
	DirectX::XMStoreFloat4x4(&sb.worldMatrix, localMatrix * worldMatrix);
	for (int i = 0; i < bindable.size(); i++)
	{
		bindable[i]->Bind(context);
	}
	dxdMain->Draw(IndexCount);
}

void Mesh::SetMaterialIDX(asMaterial* material,int idx)
{
	materialIDX = idx;
	normalTexture->ChangeNewTextureView(material->textureView);
	normalTexture->ChangeNewTextureView(material->normalView);
}
