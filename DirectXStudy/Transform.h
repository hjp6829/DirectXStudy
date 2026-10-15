#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <nlohmann/json.hpp>

class Transform : public Component
{
public:
	Transform();
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetRotaion(DirectX::XMFLOAT3 rotation);
	void SetScale(DirectX::XMFLOAT3 scale);
	DirectX::XMFLOAT3 GetObjectPosition();
	DirectX::XMFLOAT3 GetObjectRotation();
	DirectX::XMFLOAT3 GetObjectScale();
	void SetPostionOffset(DirectX::XMFLOAT3 position) { positionOffset = position; }
	void SetRotationOffset(DirectX::XMFLOAT3 rotation);
	void SetScaleOffset(DirectX::XMFLOAT3 scale) { scaleOffset = scale; }
	void SetImportLocalTransform(DirectX::XMMATRIX localMatrix);
	DirectX::XMMATRIX GetLocalMatrix() {return localMatrix; }
	void SetLocalMatrix(DirectX::XMMATRIX matrix) { localMatrix = matrix; }
	DirectX::XMMATRIX GetWorldMatrix() { return worldMatrix; }
	void SetWorldMatrix(DirectX::XMMATRIX matrix) { worldMatrix = matrix; }
	void MakeLocalMatrix();
	void Serialize(nlohmann::json& j);
	void Deserialize(const nlohmann::json& j);
	void UpdateTransformForNewParent(DirectX::FXMMATRIX oldWorldMatrix, DirectX::FXMMATRIX newParentWorldMatrix);

	DirectX::XMFLOAT3 GetForwardVector3()
	{
		DirectX::XMFLOAT3 temp;
		DirectX::XMStoreFloat3(&temp,forward);
		return temp;
	}

	// Getter
	DirectX::XMVECTOR GetUp() const { return up; }
	DirectX::XMVECTOR GetRight() const { return right; }
	DirectX::XMVECTOR GetForward() const { return forward; }

	// Setter
	void SetUp(DirectX::FXMVECTOR value) { up = value; }
	void SetRight(DirectX::FXMVECTOR value) { right = value; }
	void SetForward(DirectX::FXMVECTOR value) { forward = value; }

public:
	void InspectorUpdate() override;
private:
	void UpdateDirectionVectors();
private:
	DirectX::XMFLOAT3 positionOffset = { 0.0f, 0.0f, 4.0f };//사용자가 수정한 위차값
	DirectX::XMFLOAT4 rotationOffset = { 0.0f, 0.0f, 0.0f,0.0f };//사용자가 수정한 위차값
	DirectX::XMFLOAT3 scaleOffset = { 1.0f, 1.0f, 1.0f };//사용자가 수정한 위차값
	DirectX::XMMATRIX localMatrix;
	DirectX::XMMATRIX importLocalMatrix;
	DirectX::XMVECTOR importScale;
	DirectX::XMVECTOR importRotation;
	DirectX::XMVECTOR importPosition;
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR up;
	DirectX::XMVECTOR right;
	DirectX::XMVECTOR forward;
};