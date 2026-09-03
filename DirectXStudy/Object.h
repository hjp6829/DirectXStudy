#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Datas.h"
#include "Bindable.h"

class Bindable;
class AssimpConverter;
class ID3D11Device;
class ID3D11DeviceContext;

using namespace Microsoft::WRL;
using namespace DirectX;

class Object {
protected:
	ComPtr<ID3D11Buffer> colorConstantBuffer;
	UINT IndexCount;
public:
	Object(ID3D11Device* device, ModelLoadData* modelLoadData, AssimpConverter* assimp, asMesh* meshData) {

	}
	~Object() {}

	virtual void Update() {}
	virtual void Render(ID3D11DeviceContext* context, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix){}
	void SetPosition(float x, float y, float z) { position.x = x, position.y = y, position.z = z; }
	void SetRotation(float x, float y, float z) { rotation.x = x, rotation.y = y, rotation.z = z; }
	void SetScale(float x, float y, float z) { scale.x = x, scale.y = y, scale.z = z; }
	XMMATRIX GetLocalMatrix(){return localMatrix; }
	ConstantBufferData* GetVertexConstantBuffer() {
		return &sb;
	}
	bool enable = true;
protected:
	XMFLOAT3 position= { 0.0f, 0.0f, 4.0f };
	XMFLOAT3 rotation;
	XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
	XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMMATRIX AssimpNodeMatrix;
	ConstantBufferData sb = {};
	PSBuffer cb = {};
	XMMATRIX localMatrix;
	std::vector<std::unique_ptr<Bindable>> bindable;

};