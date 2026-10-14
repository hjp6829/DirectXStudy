#pragma once
#include "Object.h"

class CameraObject : public Object {
public:
	CameraObject();
	DirectX::XMMATRIX GetProjectionMatrix();
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMFLOAT3 GetWorldPos();
	DirectX::XMFLOAT4 GetCameraPos()
	{
		DirectX::XMFLOAT4 pos(
			positionOffset.x,
			positionOffset.y,
			positionOffset.z,
			1.0f
		);
		return pos;
	}
	void CameraTransformUpdate();
	void DrawInspectorContents() override;
	void Serialize(nlohmann::json& j) override;
	void Deserialize(const nlohmann::json& j) override;
	void UpdateObject() override;
	void MouseWheelPosUpdate();
	XMFLOAT3 GetCameraForward(){XMFLOAT3 forwardFloat3; XMStoreFloat3(&forwardFloat3, forward); return forwardFloat3; }
private:
	float FOV;
	float nearPlane = 0.5f;
	float farPlane = 1000.0f;
	float rotationSpeed = 0.5f;
	float mouseSpeed = 0.03;
	float mouseWheelSpeed = 0.2;
	float InputSpeed = 0.1;
};