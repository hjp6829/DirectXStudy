#include "TestScript.h"
#include "ScriptComponent.h"
#include "Object.h"
#include <DirectXMath.h>
#include "Input.h"
#include "imgui_impl_win32.h"

void TestScript::Update()
{
	ScriptComponent::Update();
	//if (Input::GetKey('W'))
	//{
	//	DirectX::XMFLOAT3 position = currentObject->GetObjectPosition();
	//	position.x += moveSpeed;
	//	currentObject->SetPosition(position);
	//}
	//if (Input::GetKey('S'))
	//{
	//	DirectX::XMFLOAT3 position = currentObject->GetObjectPosition();
	//	position.x -= moveSpeed;
	//	currentObject->SetPosition(position);
	//}
	//if (Input::GetKey('A'))
	//{
	//	DirectX::XMFLOAT3 position = currentObject->GetObjectPosition();
	//	position.z += moveSpeed;
	//	currentObject->SetPosition(position);
	//}
	//if (Input::GetKey('D'))
	//{
	//	DirectX::XMFLOAT3 position = currentObject->GetObjectPosition();
	//	position.z -= moveSpeed;
	//	currentObject->SetPosition(position);
	//}
}

void TestScript::Start()
{
	ScriptComponent::Start();
	componentName = "TestScript";
}

void TestScript::InspectorUpdate()
{
	ScriptComponent::InspectorUpdate();
	ImGui::Text("Speed");
	ImGui::InputFloat("moveSpeed",&moveSpeed);
}

std::unique_ptr<Component> TestScript::Clone()const
{
	return std::make_unique<TestScript>(*this);
}

void TestScript::Serialize(nlohmann::json& j)
{

}

void TestScript::Deserialize(const nlohmann::json& j)
{
}
