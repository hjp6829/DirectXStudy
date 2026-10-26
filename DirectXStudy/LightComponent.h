#pragma once
#include "Component.h"
#include <DirectXMath.h>

class LightComponent : public Component
{
public:
	DirectX::XMFLOAT4 GetLightColor() { return DirectX::XMFLOAT4(lightColor.x, lightColor.y, lightColor.z, 1); }
	float GetSpecularStrength() { return specularStrength; }
	float GetShininess() { return shininess; }
	float GetMaxLightDistance() { return maxLightDistance; }
	void SetColor(DirectX::XMFLOAT3 color) { lightColor = color; }
	void Serialize(nlohmann::json& j) override{};
	void Deserialize(const nlohmann::json& j) override{};
	void InspectorUpdate() override {}
private:
	DirectX::XMFLOAT3 lightColor = { 1.0f, 1.0f, 1.0f };
	float specularStrength = 0.5f;
	float shininess = 32.0f;
	float maxLightDistance = 10.0f;
};