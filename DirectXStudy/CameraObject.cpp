#include "CameraObject.h"
#include "Log.h"
#include "imgui_impl_win32.h"
#include "SceneSerializationData.h"
#include "Input.h"

CameraObject::CameraObject()
{
	objectName = "Camera";
}

DirectX::XMMATRIX CameraObject::GetProjectionMatrix()
{
	return DirectX::XMMatrixPerspectiveLH(1.0f, 4.0f / 3.0f, nearPlane, farPlane);
}
DirectX::XMMATRIX CameraObject::GetViewMatrix()
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR scale;

	XMMatrixDecompose(&scale, &rotation, &position, transform->GetWorldMatrix());
	DirectX::XMVECTOR Temp = rotation* position;

	XMMATRIX cameraWorld =XMMatrixRotationQuaternion(rotation) * XMMatrixTranslationFromVector(position);

	return DirectX::XMMatrixInverse(nullptr,cameraWorld);
}
void CameraObject::MouseWheelPosUpdate()
{
	DirectX::XMFLOAT3 position = transform->GetObjectPosition();
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorScale(transform->GetForward(), Input::mouseWheelDelta()), mouseWheelSpeed));
	XMFLOAT3 positionTemp;
	DirectX::XMStoreFloat3(&positionTemp, pos);
	transform->SetPosition(positionTemp);
}
void CameraObject::CameraTransformUpdate()
{
	POINT mousePos = Input::mousePositionDelta();

	if (Input::GetMouseButton(1))
	{
		XMFLOAT3 rotationOffsetTemp = transform->GetObjectRotation();
		rotationOffsetTemp.x = rotationOffsetTemp.x - mousePos.y * rotationSpeed;
		rotationOffsetTemp.y = rotationOffsetTemp.y - mousePos.x * rotationSpeed;

		DirectX::XMFLOAT3 position = transform->GetObjectPosition();
		DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
		if (Input::GetKey('W'))
			pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(transform->GetForward(), mouseWheelSpeed));
		if (Input::GetKey('S'))
			pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(transform->GetForward(), -mouseWheelSpeed));
		if (Input::GetKey('A'))
			pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(transform->GetRight(), -mouseWheelSpeed));
		if (Input::GetKey('D'))
			pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(transform->GetRight(), mouseWheelSpeed));
		XMFLOAT3 positionTemp;
		DirectX::XMStoreFloat3(&positionTemp, pos);
		transform->SetPosition(positionTemp);
		transform->SetRotaion(rotationOffsetTemp);
		return;
	}
	if (!Input::GetMouseButton(2))
		return;
	DirectX::XMFLOAT3 position = transform->GetObjectPosition();
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorScale(transform->GetRight(), mousePos.x), mouseSpeed));
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorScale(transform->GetUp(), mousePos.y), mouseSpeed));
	XMFLOAT3 positionTemp;
	DirectX::XMStoreFloat3(&positionTemp, pos);
	transform->SetPosition(positionTemp);
}

void CameraObject::Serialize(nlohmann::json& j)
{
	Object::Serialize(j);

	j["ObjectType"] = ObjectSaveType::CameraObject;
	j["isRootObject"] = 1;
}

void CameraObject::Deserialize(const nlohmann::json& j)
{
	Object::Deserialize(j);
}

void CameraObject::UpdateObject()
{	
	Object::UpdateObject();
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;
	CameraTransformUpdate();
	MouseWheelPosUpdate();
}
