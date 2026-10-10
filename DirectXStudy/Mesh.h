#pragma once
#include <DirectXMath.h>
#include "Datas.h"

class AssimpConverter;
template<typename T>
class VertexConstantBufferContainer;
class ModelLoadData;
class AssimpConverter;
class asMesh;
class ID3D11Device;
class ID3D11DeviceContext;
class Texture;
class asMaterial;
class Bindable;
class PixelShader;

using namespace DirectX;

class Mesh {
public:
	Mesh(ID3D11Device* device, ModelLoadData* modelLoadData, AssimpConverter* assimp, asMesh* meshData);
	~Mesh();
	void Render(ID3D11DeviceContext* context, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	void OutlineRender(ID3D11DeviceContext* context, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	int GetMaterialIDX(){return materialIDX; }
	void SetMaterialIDX(asMaterial* material, int idx);
	XMFLOAT3 GetLocalPos();
	void SetPosition(float x, float y, float z) { position.x = x, position.y = y, position.z = z; }
	void SetRotation(float x, float y, float z) { rotation.x = x, rotation.y = y, rotation.z = z; }
	void SetScale(float x, float y, float z) { scale.x = x, scale.y = y, scale.z = z; }
	XMMATRIX GetLocalMatrix() { return localMatrix; }
	ConstantBufferData* GetVertexConstantBuffer() {
		return &sb;
	}
	bool enable = true;
private:
	Texture* meshTexture;
	Texture* normalTexture;
	int materialIDX;
	VertexConstantBufferContainer<ConstantBufferData>* vertexConstantBufferContainer;
	XMFLOAT3 position = { 0.0f, 0.0f, 4.0f };
	XMFLOAT3 rotation;
	XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
	XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMMATRIX AssimpNodeMatrix;
	ConstantBufferData sb = {};
	PSBuffer cb = {};
	XMMATRIX localMatrix;
	std::unique_ptr<PixelShader> outlineShader;
	std::vector<std::unique_ptr<Bindable>> bindable;
	std::vector<Bindable*> outlineBindable;
	UINT IndexCount;
};