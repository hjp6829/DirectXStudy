#include "LightObject.h"
#include "imgui_impl_win32.h"
#include "SceneSerializationData.h"

void LightObject::DrawInspectorContents()
{
	Object::DrawInspectorContents();
	ImGui::InputFloat("Specular Strength", &specularStrength);
	ImGui::InputFloat("Shininess", &shininess);
	ImGui::InputFloat("Max Light Distance", &maxLightDistance);
	ImGui::Text("Color");
	if (ImGui::InputFloat3("Color", &lightColor.x))
		SetColor(lightColor);
}

void LightObject::Serialize(nlohmann::json& j)
{
	Object::Serialize(j);
	j["lightColorR"] = lightColor.x;
	j["lightColorG"] = lightColor.y;
	j["lightColorB"] = lightColor.z;
	j["specularStrength"] = specularStrength;
	j["shininess"] = shininess;
	j["maxLightDistance"] = maxLightDistance;
	j["ObjectType"] = ObjectSaveType::LightObject;
}