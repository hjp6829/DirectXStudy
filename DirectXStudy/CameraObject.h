#pragma once
#include "Object.h"

class CameraObject : public Object {
public:
	CameraObject();
	DirectX::XMMATRIX GetProjectionMatrix();
	DirectX::XMMATRIX GetViewMatrix();
	void CameraTransformUpdate();
	void Serialize(nlohmann::json& j) override;
	void Deserialize(const nlohmann::json& j) override;
	void UpdateObject() override;
	void MouseWheelPosUpdate();
	bool CheckSceneCamera(){return isSceneCamera;}
	void SetIsSceneCamera(bool value){ isSceneCamera = value;}
private:
	float FOV;
	float nearPlane = 0.5f;
	float farPlane = 1000.0f;
	float rotationSpeed = 0.1f;
	float mouseSpeed = 0.03;
	float mouseWheelSpeed = 0.2;
	float InputSpeed = 0.1;
	bool isSceneCamera = false;
};