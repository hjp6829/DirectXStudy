#pragma once
#include "Component.h"
#include <DirectXMath.h>

class SceneCameraComponent : public Component
{
public:
	void InspectorUpdate() override{}
	void Serialize(nlohmann::json& j) override{};
	void Deserialize(const nlohmann::json& j) override{};
	void LateUpdate() override;
	void MouseWheelPosUpdate();
	void CameraTransformUpdate();
private:
	float rotationSpeed = 0.1f;
	float mouseSpeed = 0.03;
	float mouseWheelSpeed = 0.2;
};