#include "CameraComponent.h"
#include "Object.h"
#include "imgui_impl_win32.h"
CameraComponent::CameraComponent()
{
	componentName = "Camera";
}
DirectX::XMMATRIX CameraComponent::GetProjectionMatrix()const
{
	return DirectX::XMMatrixPerspectiveLH(XMConvertToRadians(FOV), 3.0f / 4.0f, nearPlane, farPlane);
}
DirectX::XMMATRIX CameraComponent::GetViewMatrix()const
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR scale;

	DirectX::XMMatrixDecompose(&scale, &rotation, &position, currentObject->GetTransform()->GetWorldMatrix());
	DirectX::XMVECTOR Temp = rotation * position;

	DirectX::XMMATRIX cameraWorld = DirectX::XMMatrixRotationQuaternion(rotation) * DirectX::XMMatrixTranslationFromVector(position);

	return DirectX::XMMatrixInverse(nullptr, cameraWorld);
}

DirectX::XMFLOAT3 CameraComponent::GetCameraPosition() const
{
	return currentObject->GetTransform()->GetObjectPosition();
}

DirectX::XMFLOAT3 CameraComponent::GetCameraForward() const
{
	return currentObject->GetTransform()->GetForwardVector3();
}

void CameraComponent::InspectorUpdate()
{
	Component::InspectorUpdate();

	ImGui::InputFloat("FOV",&FOV);
}

std::unique_ptr<Component> CameraComponent::Clone() const
{
	std::unique_ptr<CameraComponent> Temp = std::make_unique<CameraComponent>(*this);
	return std::move(Temp);
}
void CameraComponent::Serialize(nlohmann::json& j)
{
	j = nlohmann::json{
		{"FOV",FOV},
		{"ComponentName","CameraComponent"}
	};
}

void CameraComponent::Deserialize(const nlohmann::json& j)
{
	FOV = j.at("FOV").get<float>();
}
