#pragma once
#include "Object.h"
#include <nlohmann/json.hpp>
class LightObject : public Object{
public:
	LightObject(){};
	DirectX::XMFLOAT4 GetLightPos() { return DirectX::XMFLOAT4(positionOffset.x, positionOffset.y, positionOffset.z, 1); }
	DirectX::XMFLOAT4 GetLightColor() { return DirectX::XMFLOAT4(lightColor.x, lightColor.y, lightColor.z, 1); }
	float GetSpecularStrength() { return specularStrength; }
	float GetShininess() { return shininess; }
	float GetMaxLightDistance() { return maxLightDistance; }
	void SetColor(DirectX::XMFLOAT3 color) { lightColor = color; }
public:
	void DrawInspectorContents() override;
	void Serialize(nlohmann::json& j) override;
	void Deserialize(const nlohmann::json& j) override;
private:
	XMFLOAT3 lightColor = { 1.0f, 1.0f, 1.0f };
	float specularStrength = 0.5f;
	float shininess = 32.0f;
	float maxLightDistance = 10.0f;
};