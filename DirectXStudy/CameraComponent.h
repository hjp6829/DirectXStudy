#pragma once
#include "Component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "ICamera.h"

class CameraComponent : public Component, public ICamera {
public:
	CameraComponent();
	void Awake() override;
	DirectX::XMMATRIX GetProjectionMatrix()const override;
	DirectX::XMMATRIX GetViewMatrix() const override;
	DirectX::XMFLOAT3 GetCameraPosition()const override;
	DirectX::XMFLOAT3 GetCameraForward() const override;
	void InspectorUpdate() override;
	std::unique_ptr<Component> Clone()const override;
	void Serialize(nlohmann::json& j) override;
	void Deserialize(const nlohmann::json& j) override;
private:
	float FOV = 60;
	float nearPlane = 0.5f;
	float farPlane = 1000.0f;
	float rotationSpeed = 0.1f;
	float mouseSpeed = 0.03;
	float mouseWheelSpeed = 0.2;
	float InputSpeed = 0.1;
};