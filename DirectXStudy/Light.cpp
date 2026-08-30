#include "Light.h"
#include "imgui_impl_win32.h"

Light::Light()
{
}

void Light::ViewLightWindow()
{
	if (ImGui::Begin("Light"))
	{
		ImGui::SliderFloat("ColorR", &colorR, 0.0f, 1.0f);
		ImGui::SliderFloat("ColorG", &colorG, 0.0f, 1.0f);
		ImGui::SliderFloat("ColorB", &colorB, 0.0f, 1.0f);

		ImGui::SliderFloat("PositionX", &posX, -10.0f, 10.0f);
		ImGui::SliderFloat("PositionY", &posY, -10.0f, 10.0f);
		ImGui::SliderFloat("PositionZ", &posZ, -10.0f, 10.0f);

		ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 5.0f);
		ImGui::SliderFloat("shininess", &shininess, 0.0f, 30.0f);
		ImGui::SliderFloat("maxLightDistance", &maxLightDistance, 0.0f, 30.0f);
	}
	ImGui::End();
}

DirectX::XMFLOAT4 Light::GetLightPos()
{
	return DirectX::XMFLOAT4(posX, posY, posZ,1);
}

DirectX::XMFLOAT4 Light::GetLightColor()
{
	return DirectX::XMFLOAT4(colorR, colorG, colorB, 1);
}
