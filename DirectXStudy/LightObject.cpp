#include "LightObject.h"
#include "imgui_impl_win32.h"
#include "SceneSerializationData.h"

LightObject::LightObject()
{
	objectName = "Light";
}

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
	j["isRootObject"] = 1;
}

void LightObject::Deserialize(const nlohmann::json& j)
{
	Object::Deserialize(j);

	lightColor.x = j["lightColorR"];
	lightColor.y = j["lightColorG"];
	lightColor.z = j["lightColorB"];
	specularStrength = j["specularStrength"];
	shininess = j["shininess"];
	maxLightDistance = j["maxLightDistance"];
}
void LightObject::SetPostionOffset(XMFLOAT3 position)
{
	Object::SetPostionOffset(position);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotationOffset.x), DirectX::XMConvertToRadians(rotationOffset.y), DirectX::XMConvertToRadians(rotationOffset.z));
	forward = XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), rotationMatrix);
}
