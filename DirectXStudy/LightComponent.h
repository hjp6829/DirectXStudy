#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "Object.h"

class LightComponent : public Component
{
public:
	LightComponent();
	void Awake() override;
	DirectX::XMFLOAT4 GetLightColor() { return DirectX::XMFLOAT4(lightColor.x, lightColor.y, lightColor.z, 1); }
	DirectX::XMFLOAT3 GetLightPosition() {return currentObject->GetTransform()->GetObjectPosition(); }
	DirectX::XMFLOAT3 GetForwardVector3() {return currentObject->GetTransform()->GetForwardVector3(); }
	float GetSpecularStrength() { return specularStrength; }
	float GetShininess() { return shininess; }
	float GetMaxLightDistance() { return maxLightDistance; }
	void SetColor(DirectX::XMFLOAT3 color) { lightColor = color; }
	void Serialize(nlohmann::json& j) override;
	void Deserialize(const nlohmann::json& j) override{};
	void InspectorUpdate() override {}
	std::unique_ptr<Component> Clone()const override;
private:
	DirectX::XMFLOAT3 lightColor = { 1.0f, 1.0f, 1.0f };
	float specularStrength = 0.5f;
	float shininess = 32.0f;
	float maxLightDistance = 10.0f;
};