#include "LightObject.h"
#include "AssimpConverter.h"

LightObject::LightObject(DirectXMain* dxdMain, ModelLoadData* modelLoadData, AssimpConverter* assimp, asMesh* meshData) : Object(dxdMain, modelLoadData, assimp, meshData)
{
	this->dxdMain = dxdMain;
	SetScale(0.2f, 0.2f, 0.2f);

	ComPtr<ID3DBlob> vertexShaderBlob;
	D3DReadFileToBlob(L"VertexShader.cso", &vertexShaderBlob);
	bindable.push_back(std::make_unique<VertexShader>(*dxdMain, vertexShaderBlob.Get()));

	bindable.push_back(std::make_unique<InputLayout>(*dxdMain, vertexShaderBlob.Get()));

	ComPtr<ID3DBlob> pixelShaderBlob;
	D3DReadFileToBlob(L"LightPS.cso", &pixelShaderBlob);
	bindable.push_back(std::make_unique<PixelShader>(*dxdMain, pixelShaderBlob.Get()));

	float heightUV = 1.0f / 4.0f;
	float widthUV = 1.0f / 3.0f;

	XMVECTOR frontVector1 = XMVectorSet(-0.5f, -0.5f, -0.5f, 0.0f) - XMVectorSet(-0.5f, 0.5f, -0.5f, 0.0f);
	XMVECTOR frontVector2 = XMVectorSet(0.5f, 0.5f, -0.5f, 0.0f) - XMVectorSet(-0.5f, 0.5f, -0.5f, 0.0f);
	XMVECTOR frontNormal = XMVector3Cross(frontVector2, frontVector1);
	XMFLOAT3 frontNormalFloat3;
	XMStoreFloat3(&frontNormalFloat3, frontNormal);

	XMVECTOR upVector1 = XMVectorSet(-0.5f, 0.5f, -0.5f, 0.0f) - XMVectorSet(-0.5f, 0.5f, 0.5f, 0.0f);
	XMVECTOR upVector2 = XMVectorSet(0.5f, 0.5f, 0.5f, 0.0f) - XMVectorSet(-0.5f, 0.5f, 0.5f, 0.0f);
	XMVECTOR upNormal = XMVector3Cross(upVector2, upVector1);
	XMFLOAT3 upNormalFloat3;
	XMStoreFloat3(&upNormalFloat3, upNormal);

	XMVECTOR backVector1 = XMVectorSet(-0.5f, 0.5f, 0.5f, 0.0f) - XMVectorSet(-0.5f, -0.5f, 0.5f, 0.0f);
	XMVECTOR backVector2 = XMVectorSet(0.5f, -0.5f, 0.5f, 0.0f) - XMVectorSet(-0.5f, -0.5f, 0.5f, 0.0f);
	XMVECTOR backNormal = XMVector3Cross(backVector2, backVector1);
	XMFLOAT3 backNormalFloat3;
	XMStoreFloat3(&backNormalFloat3, backNormal);

	XMVECTOR downVector1 = XMVectorSet(0.5f, -0.5f, 0.5f, 0.0f) - XMVectorSet(-0.5f, -0.5f, 0.5f, 0.0f);
	XMVECTOR downVector2 = XMVectorSet(0.5f, -0.5f, -0.5f, 0.0f) - XMVectorSet(-0.5f, -0.5f, 0.5f, 0.0f);
	XMVECTOR downNormal = XMVector3Cross(downVector2, downVector1);
	XMFLOAT3 downNormalFloat3;
	XMStoreFloat3(&downNormalFloat3, downNormal);

	XMVECTOR leftVector1 = XMVectorSet(-0.5f, 0.5f, 0.5f, 0.0f) - XMVectorSet(-0.5f, -0.5f, 0.5f, 0.0f);
	XMVECTOR leftVector2 = XMVectorSet(-0.5f, -0.5f, -0.5f, 0.0f) - XMVectorSet(-0.5f, -0.5f, 0.5f, 0.0f);
	XMVECTOR leftNormal = XMVector3Cross(leftVector1, leftVector2);
	XMFLOAT3 leftNormalFloat3;
	XMStoreFloat3(&leftNormalFloat3, leftNormal);

	XMVECTOR rightVector1 = XMVectorSet(0.5f, -0.5f, 0.5f, 0.0f) - XMVectorSet(0.5f, 0.5f, 0.5f, 0.0f);
	XMVECTOR rightVector2 = XMVectorSet(0.5f, -0.5f, -0.5f, 0.0f) - XMVectorSet(0.5f, 0.5f, 0.5f, 0.0f);
	XMVECTOR rightNormal = XMVector3Cross(rightVector1, rightVector2);
	XMFLOAT3 rightNormalFloat3;
	XMStoreFloat3(&rightNormalFloat3, rightNormal);

	std::vector<Vertex> vertexs = {
		//후면
	{{ -0.5f,-0.5f,0.5f} , {widthUV,heightUV * 3} , backNormalFloat3},
	{{ -0.5f,0.5f,0.5f}, {widthUV,heightUV * 2},backNormalFloat3},
	{{ 0.5f,0.5f,0.5f }, {widthUV * 2,heightUV * 2},backNormalFloat3},
	{{ 0.5f,-0.5f,0.5f}, {widthUV * 2,heightUV * 3},backNormalFloat3},

	//윗면
	{{ -0.5f,0.5f,0.5f },{widthUV,heightUV * 2},upNormalFloat3},
	{{-0.5f,0.5f,-0.5f},{widthUV,heightUV},upNormalFloat3},
	{{0.5f,0.5f,-0.5f},{widthUV * 2,heightUV},upNormalFloat3},
	{{ 0.5f,0.5f,0.5f },{widthUV * 2,heightUV * 2},upNormalFloat3} ,

	//정면
	{ {-0.5f,0.5f,-0.5f},{widthUV,heightUV} ,frontNormalFloat3},//1
	{{-0.5f,-0.5f,-0.5f},{widthUV,0},frontNormalFloat3},//2
	{{0.5f,-0.5f,-0.5f},{widthUV * 2,0},frontNormalFloat3},//3
	{{0.5f,0.5f,-0.5f},{widthUV * 2,heightUV},frontNormalFloat3},//4
	//
	//밑판
	{{ -0.5f,-0.5f,0.5f},{widthUV,heightUV * 3} ,downNormalFloat3},
	{{0.5f,-0.5f,0.5f},{widthUV * 2,heightUV * 3},downNormalFloat3},
	{{-0.5f,-0.5f,-0.5f},{widthUV,1},downNormalFloat3},
	{{0.5f,-0.5f,-0.5f},{widthUV * 2,1},downNormalFloat3},
	//
	//왼
	{{ -0.5f,-0.5f,0.5f},{widthUV,heightUV * 3} ,leftNormalFloat3},
	{{-0.5f,0.5f,0.5f},{widthUV,heightUV * 2},leftNormalFloat3},
	{{-0.5f,0.5f,-0.5f},{0,heightUV * 2},leftNormalFloat3},
	{{-0.5f,-0.5f,-0.5f},{0 ,heightUV * 3},leftNormalFloat3},
	//
	//오
	{{0.5f,0.5f,0.5f},{widthUV * 2,heightUV * 2} ,rightNormalFloat3},
	{{0.5f,-0.5f,0.5f},{widthUV * 2,heightUV * 3},rightNormalFloat3},
	{{0.5f,0.5f,-0.5f},{1,heightUV * 2},rightNormalFloat3},
	{{0.5f,-0.5f,-0.5f},{1 ,heightUV * 3},rightNormalFloat3},
	};
	bindable.push_back(std::make_unique<VertexBuffer>(*dxdMain, vertexs));

	const std::vector<unsigned short> indices = {
		0,2,1,
		0,3,2,
		4,6,5,
		4,7,6,
		8,10,9,
		8,11,10,
		12,14,13,
		13,14,15,
		16,17,19,
		19,17,18,
		20,21,22,
		23,22,21
	};

	std::unique_ptr<IndexBuffer> indexBuffer = std::make_unique<IndexBuffer>(*dxdMain, indices);
	IndexCount = indexBuffer->GetIndexCount();

	bindable.push_back(std::move(indexBuffer));

	bindable.push_back(std::make_unique<PrimitiveTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	localMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) *
		XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
		XMMatrixTranslation(position.x, position.y, position.z);
	//bindable.push_back(std::make_unique<VertexConstantBufferContainer<ConstantBuffer>>(*dxdMain, sb, *this));
}

void LightObject::Update(XMMATRIX worldMatrix)
{
	DirectX::XMFLOAT4 lightPos = dxdMain->GetLight()->GetLightPos();
	SetPosition(lightPos.x, lightPos.y, lightPos.z);
	Object::Update(worldMatrix);
}

void LightObject::Render(DirectXMain* dxdMain)
{
	//for (int i = 0; i < bindable.size(); i++)
	//{
	//	bindable[i]->Bind(*dxdMain);
	//}

	dxdMain->Draw(IndexCount);
}
