#pragma once
#include "Component.h"
#include <DirectXMath.h>

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
	void SetRotationOffset(DirectX::XMFLOAT4 rotation);
	void SetScaleOffset(DirectX::XMFLOAT3 scale) { scaleOffset = scale; }
	void SetImportLocalMatrix(DirectX::XMMATRIX localMatrix);
	DirectX::XMMATRIX GetLocalMatrix() {return localMatrix; }
	void SetLocalMatrix(DirectX::XMMATRIX matrix) { localMatrix = matrix; }
	DirectX::XMMATRIX GetWorldMatrix() { return worldMatrix; }
	void SetWorldMatrix(DirectX::XMMATRIX matrix) { worldMatrix = matrix; }
	void MakeLocalMatrix();
	void UpdateTransformForNewParent(DirectX::FXMMATRIX oldWorldMatrix, DirectX::FXMMATRIX newParentWorldMatrix);
	DirectX::XMMATRIX GetImportLocalMatrix() { return importLocalMatrix; }
	DirectX::XMMATRIX GetOffsetMatrix(){return offsetMatrix; }
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
	void CopyTransform(Transform* origTransform);

public:
	void InspectorUpdate() override;
	std::unique_ptr<Component> Clone()const override;
	void Serialize(nlohmann::json& j) override;
	void Deserialize(const nlohmann::json& j) override;
private:
	void UpdateDirectionVectors();
private:
	DirectX::XMFLOAT3 positionOffset = { 0.0f, 0.0f, 0.0f };//����ڰ� ������ ������
	DirectX::XMFLOAT4 rotationOffset = { 0.0f, 0.0f, 0.0f,1.0f };//����ڰ� ������ ������
	DirectX::XMFLOAT3 scaleOffset = { 1.0f, 1.0f, 1.0f };//����ڰ� ������ ������
	DirectX::XMMATRIX localMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX importLocalMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR importScale;
	DirectX::XMVECTOR importRotation;
	DirectX::XMVECTOR importPosition;
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR up;
	DirectX::XMVECTOR right;
	DirectX::XMVECTOR forward;
	DirectX::XMMATRIX offsetMatrix;
};