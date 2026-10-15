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
	XMVECTOR pos = XMLoadFloat3(&transform->GetPostionOffset());
	XMFLOAT3 rotationOffsetTemp = transform->GetRotationOffset();
	return DirectX::XMMatrixInverse(NULL, DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotationOffsetTemp.x), DirectX::XMConvertToRadians(rotationOffsetTemp.y), DirectX::XMConvertToRadians(rotationOffsetTemp.z)) * DirectX::XMMatrixTranslationFromVector(pos));
}
void CameraObject::MouseWheelPosUpdate()
{
	XMFLOAT3 rotationOffsetTemp = transform->GetRotationOffset();
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotationOffsetTemp.x), DirectX::XMConvertToRadians(rotationOffsetTemp.y), DirectX::XMConvertToRadians(rotationOffsetTemp.z));
	transform->SetForward(XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), rotationMatrix));

	XMVECTOR pos = XMLoadFloat3(&transform->GetPostionOffset());
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorScale(transform->GetForward(), Input::mouseWheelDelta()), mouseWheelSpeed));
	XMFLOAT3 positionTemp;
	DirectX::XMStoreFloat3(&positionTemp, pos);
	transform->SetPostionOffset(positionTemp);
}
void CameraObject::CameraTransformUpdate()
{
	POINT mousePos = Input::mousePositionDelta();

	if (Input::GetMouseButton(1))
	{
		XMFLOAT3 rotationOffsetTemp = transform->GetRotationOffset();
		rotationOffsetTemp.x = rotationOffsetTemp.x + mousePos.y * rotationSpeed;
		rotationOffsetTemp.y = rotationOffsetTemp.y - mousePos.x * rotationSpeed;

		XMVECTOR pos = XMLoadFloat3(&transform->GetPostionOffset());
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
		transform->SetPostionOffset(positionTemp);
		transform->SetRotationOffset(rotationOffsetTemp);
		return;
	}
	if (!Input::GetMouseButton(2))
		return;
	XMVECTOR pos = XMLoadFloat3(&transform->GetPostionOffset());
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorScale(transform->GetRight(), mousePos.x), mouseSpeed));
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorScale(transform->GetUp(), mousePos.y), mouseSpeed));
	XMFLOAT3 positionTemp;
	DirectX::XMStoreFloat3(&positionTemp, pos);
	transform->SetPostionOffset(positionTemp);
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
