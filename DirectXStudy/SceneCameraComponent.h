#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "ICamera.h"

class SceneCameraComponent : public Component
{
public:
	SceneCameraComponent();
	void Awake() override;
	void InspectorUpdate() override{}
	void Serialize(nlohmann::json& j) override;
	void Deserialize(const nlohmann::json& j) override{};
	void LateUpdate() override;
	void MouseWheelPosUpdate();
	void CameraTransformUpdate();
	std::unique_ptr<Component> Clone()const override;
private:
	float rotationSpeed = 0.1f;
	float mouseSpeed = 0.03;
	float mouseWheelSpeed = 0.2;
};