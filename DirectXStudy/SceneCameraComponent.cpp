#include "SceneCameraComponent.h"
#include "Input.h"
#include "Object.h"
#include "imgui_impl_win32.h"

void SceneCameraComponent::LateUpdate()
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;
	MouseWheelPosUpdate();
	CameraTransformUpdate();
}

void SceneCameraComponent::MouseWheelPosUpdate()
{
	DirectX::XMFLOAT3 position = currentObject->GetTransform()->GetObjectPosition();
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorScale(currentObject->GetTransform()->GetForward(), Input::mouseWheelDelta()), mouseWheelSpeed));
	DirectX::XMFLOAT3 positionTemp;
	DirectX::XMStoreFloat3(&positionTemp, pos);
	currentObject->GetTransform()->SetPosition(positionTemp);
}
void SceneCameraComponent::CameraTransformUpdate()
{
	POINT mousePos = Input::mousePositionDelta();

	if (Input::GetMouseButton(1))
	{
		DirectX::XMFLOAT3 cameraRotation = currentObject->GetTransform()->GetObjectRotation();

		cameraRotation.x = cameraRotation.x + mousePos.y * rotationSpeed;
		cameraRotation.y = cameraRotation.y - mousePos.x * rotationSpeed;

		currentObject->GetTransform()->SetRotaion(cameraRotation);

		DirectX::XMFLOAT3 position = currentObject->GetTransform()->GetObjectPosition();
		DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);

		const DirectX::XMVECTOR forward = XMVector3Normalize(currentObject->GetTransform()->GetForward());
		const DirectX::XMVECTOR right = XMVector3Normalize(currentObject->GetTransform()->GetRight());

		if (Input::GetKey('W'))
			pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(forward, mouseWheelSpeed));
		if (Input::GetKey('S'))
			pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(forward, -mouseWheelSpeed));
		if (Input::GetKey('A'))
			pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(right, -mouseWheelSpeed));
		if (Input::GetKey('D'))
			pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(right, mouseWheelSpeed));

		DirectX::XMFLOAT3 positionTemp;
		DirectX::XMStoreFloat3(&positionTemp, pos);
		currentObject->GetTransform()->SetPosition(positionTemp);

		return;
	}
	if (!Input::GetMouseButton(2))
		return;
	DirectX::XMFLOAT3 position = currentObject->GetTransform()->GetObjectPosition();
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorScale(currentObject->GetTransform()->GetRight(), mousePos.x), mouseSpeed));
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorScale(currentObject->GetTransform()->GetUp(), mousePos.y), mouseSpeed));
	DirectX::XMFLOAT3 positionTemp;
	DirectX::XMStoreFloat3(&positionTemp, pos);
	currentObject->GetTransform()->SetPosition(positionTemp);
}