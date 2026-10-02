#include "CameraObject.h"
#include "Log.h"
#include "imgui_impl_win32.h"
#include "SceneSerializationData.h"

DirectX::XMMATRIX CameraObject::GetProjectionMatrix()
{
	return DirectX::XMMatrixPerspectiveLH(1.0f, 4.0f / 3.0f, nearPlane, farPlane);
}
DirectX::XMFLOAT3 CameraObject::GetWorldPos()
{
	return positionOffset;
}
DirectX::XMMATRIX CameraObject::GetViewMatrix()
{
	XMVECTOR pos = XMLoadFloat3(&positionOffset);
	return DirectX::XMMatrixInverse(NULL, DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotationOffset.x), DirectX::XMConvertToRadians(rotationOffset.y), DirectX::XMConvertToRadians(rotationOffset.z)) * DirectX::XMMatrixTranslationFromVector(pos));
}
void CameraObject::SetMouseWheelDelta(int value)
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotationOffset.x), DirectX::XMConvertToRadians(rotationOffset.y), DirectX::XMConvertToRadians(rotationOffset.z));
	forward = XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), rotationMatrix);

	XMVECTOR pos = XMLoadFloat3(&positionOffset);
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorScale(forward, value), mouseWheelSpeed));
	XMStoreFloat3(&positionOffset, pos);
}
void CameraObject::SetMouseDeltaPos(int x, int y)
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotationOffset.x), DirectX::XMConvertToRadians(rotationOffset.y), DirectX::XMConvertToRadians(rotationOffset.z));
	up = XMVector3TransformNormal(DirectX::XMVectorSet(0, 1, 0, 0), rotationMatrix);
	right = XMVector3TransformNormal(DirectX::XMVectorSet(1, 0, 0, 0), rotationMatrix);
	forward = XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), rotationMatrix);
	if (isMouseRight)
	{
		rotationOffset.x = rotationOffset.x + y * rotationSpeed;
		rotationOffset.y = rotationOffset.y - x * rotationSpeed;
		return;
	}
	if (!isMouseWheelDown)
		return;
	XMVECTOR pos = XMLoadFloat3(&positionOffset);
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorScale(right, x), mouseSpeed));
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(DirectX::XMVectorScale(up,y), mouseSpeed));
	XMStoreFloat3(&positionOffset, pos);
}
void CameraObject::KeyboardEvent(int idx)
{
	if (!isMouseRight)
		return;
	XMVECTOR pos = XMLoadFloat3(&positionOffset);
	if ((char)idx == 'W')
		pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(forward, mouseWheelSpeed));
	if ((char)idx == 'S')
		pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(forward, -mouseWheelSpeed));
	if ((char)idx == 'A')
		pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(right, -mouseWheelSpeed));
	if ((char)idx == 'D')
		pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(right, mouseWheelSpeed));
	XMStoreFloat3(&positionOffset, pos);
}

void CameraObject::DrawInspectorContents()
{
	Object::DrawInspectorContents();

}

void CameraObject::Serialize(nlohmann::json& j)
{
	Object::Serialize(j);
	j["ObjectType"] = ObjectSaveType::CameraObject;
}
