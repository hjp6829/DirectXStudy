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
	commonBindables.push_back(std::make_unique<VertexShader>(device, vertexShaderBlob.Get()));

	normalBindables.push_back(std::make_unique<Texture>(material->textureView, 0));

	normalBindables.push_back(std::make_unique<Sampler>(device, 0));

	normalBindables.push_back(std::make_unique<Texture>(material->normalView, 1));
	normalBindables.push_back(std::make_unique<Sampler>(device, 1));

	commonBindables.push_back(std::make_unique<InputLayout>(device, vertexShaderBlob.Get()));

	ComPtr<ID3DBlob> pixelShaderBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &pixelShaderBlob);
	normalShader = std::make_unique<PixelShader>(device, pixelShaderBlob.Get());

	ComPtr<ID3DBlob> outlineShaderBlob;
	D3DReadFileToBlob(L"OutlineShader.cso", &outlineShaderBlob);
	outlineShader = std::make_unique<PixelShader>(device, outlineShaderBlob.Get());

	ComPtr<ID3DBlob> pickingShaderBlob;
	D3DReadFileToBlob(L"PickingPixelShader.cso", &pickingShaderBlob);
	pickingShader = std::make_unique<PixelShader>(device, pickingShaderBlob.Get());

	commonBindables.push_back(std::make_unique<VertexBuffer>(device, meshData->vertexs));

	auto indexBuffer = std::make_unique<IndexBuffer>(
		device, meshData->Indexs
	);
	IndexCount = indexBuffer->GetIndexCount();
	commonBindables.push_back(std::move(indexBuffer));

	commonBindables.push_back(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	localMatrix = 	XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
		XMMatrixTranslation(position.x, position.y, position.z);

	cb.color = color;
	normalBindables.push_back(std::make_unique<PSContantBuffer<PSBuffer>>(device, cb));

	auto vertexConstantBufferTemp =
		std::make_unique<VertexConstantBufferContainer<ConstantBufferData>>(
			device, sb, *this
		);

	vertexConstantBufferContainer = vertexConstantBufferTemp.get();
	commonBindables.push_back(std::move(vertexConstantBufferTemp));

	D3D11_BUFFER_DESC sDes = {};
	sDes.ByteWidth = sizeof(pickingIDBufferData);
	sDes.Usage = D3D11_USAGE_DYNAMIC;
	sDes.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	sDes.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sDes.MiscFlags = 0;
	sDes.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = &pickingIDBufferData;

	device->CreateBuffer(&sDes, &sd, pickingConstantBuffer.GetAddressOf());
}

void Mesh::Render(RenderPass renderPass, ID3D11DeviceContext* context, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, int ObjectID)
{
	if (!enable)
		return;

	DirectX::XMStoreFloat4x4(&sb.worldMatrix, localMatrix * worldMatrix);
	vertexConstantBufferContainer->SetModelMatrix(viewMatrix, projectionMatrix);

	for (int i = 0; i < commonBindables.size(); i++)
	{
		commonBindables[i]->Bind(context);
	}

	switch (renderPass)
	{
	case RenderPass::Main:
		normalShader->Bind(context);
		for (int i = 0; i < normalBindables.size(); i++)
		{
			normalBindables[i]->Bind(context);
		}
		break;
	case RenderPass::Outline:
		outlineShader->Bind(context);
		break;
	case RenderPass::Picking:
		{
		pickingIDBufferData.id = ObjectID;
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		HRESULT hr = context->Map(pickingConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		if (FAILED(hr))
			return;
		memcpy(mapped.pData, &pickingIDBufferData, sizeof(pickingIDBufferData));
		context->Unmap(pickingConstantBuffer.Get(), 0);
		context->PSSetConstantBuffers(0, 1, pickingConstantBuffer.GetAddressOf());
		pickingShader->Bind(context);
		}
		break;
	default:
		return;
	}
	context->DrawIndexed(IndexCount,0,0);
}

void Mesh::SetMaterialIDX(asMaterial* material,int idx)
{
	//materialIDX = idx;
	//meshTexture->ChangeNewTextureView(material->textureView);
	//normalTexture->ChangeNewTextureView(material->normalView);
}

XMFLOAT3 Mesh::GetLocalPos()
{
	DirectX::XMVECTOR positionVec = localMatrix.r[3];

	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, positionVec);
	return position;
}
