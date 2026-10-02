#include "LightObject.h"
#include "imgui_impl_win32.h"

void LightObject::ShowInspectorUI()
{
	Object::ShowInspectorUI();
	if (ImGui::Begin("Light"))
	{
		ImGui::InputFloat("Specular Strength", &specularStrength);
		ImGui::InputFloat("Shininess", &shininess);
		ImGui::InputFloat("Max Light Distance", &maxLightDistance);
		ImGui::Text("Color");
		if (ImGui::InputFloat3("Color", &lightColor.x))
			SetColor(lightColor);
	}
	ImGui::End();
}

void LightObject::Serialize(nlohmann::json& j)
{
	Object::Serialize(j);
	j = nlohmann::json{
		{"lightColorR", lightColor.x},
		{"lightColorG", lightColor.y},
		{"lightColorB", lightColor.z},
		{"specularStrength", specularStrength},
		{"shininess", shininess},
		{"maxLightDistance", maxLightDistance},
	};
}
