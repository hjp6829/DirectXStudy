#pragma once
#include "Object.h"
#include "ICamera.h"

class CameraObject : public Object , public ICamera{
public:
	CameraObject();
	DirectX::XMMATRIX GetProjectionMatrix() const override;
	DirectX::XMMATRIX GetViewMatrix()const override;
	DirectX::XMFLOAT3 GetCameraPosition()const override;
	DirectX::XMFLOAT3 GetCameraForward() const override;
	void CameraTransformUpdate();
	void Serialize(nlohmann::json& j) override;
	void Deserialize(const nlohmann::json& j) override;
	void UpdateObject() override;
	void MouseWheelPosUpdate();
	bool CheckSceneCamera(){return isSceneCamera;}
	void SetIsSceneCamera(bool value){ isSceneCamera = value;}
private:
	float FOV = 60;
	float nearPlane = 0.5f;
	float farPlane = 1000.0f;
	float rotationSpeed = 0.1f;
	float mouseSpeed = 0.03;
	float mouseWheelSpeed = 0.2;
	float InputSpeed = 0.1;
	bool isSceneCamera = false;
};