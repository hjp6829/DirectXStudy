#pragma once
#include <DirectXMath.h>

class Light {
public:
	Light();
	void ViewLightWindow();
	DirectX::XMFLOAT4 GetLightPos();
	DirectX::XMFLOAT4 GetLightColor();
	float GetSpecularStrength() { return specularStrength; }
	float GetShininess() { return shininess; }
	float GetMaxLightDistance() { return maxLightDistance; }
private:
	float posX = 0, posY = 0, posZ = 0;
	float colorR = 1, colorG = 1, colorB = 1;
	float specularStrength = 0.5f;
	float shininess = 32.0f;
	float maxLightDistance = 10.0f;
};