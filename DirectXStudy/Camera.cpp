#include "Camera.h"
#include "imgui_impl_win32.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Log.h"
#include <string>
Camera::Camera(Keyboard* keyboard)
{
	keyboard->OnKeyHold = [this](int key)
		{
			this->KeyboardEvent(key);
		};
}

void Camera::ViewCameraWindow()
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::SliderFloat("RotationX", &rotationX, -180.0f, 180.0f);
		ImGui::SliderFloat("RotationY", &rotationY, -180.0f, 180.0f);
		ImGui::SliderFloat("RotationZ", &rotationZ, -180.0f, 180.0f);

		//ImGui::SliderFloat("PositionX", &posX, -10.0f, 10.0f);
		//ImGui::SliderFloat("PositionY", &posY, -10.0f, 10.0f);
		//ImGui::SliderFloat("PositionZ", &posZ, -10.0f, 10.0f);

		ImGui::SliderFloat("KeyboardSpeed", &KeyboardSpeed, 0.0f, 0.3f);
		ImGui::SliderFloat("MouseSpeed", &mouseSpeed, 0.0f, 0.3f);
		ImGui::SliderFloat("MouseWheelSpeed", &mouseWheelSpeed, 0.0f, 0.3f);
		ImGui::SliderFloat("RotationSpeed", &rotationSpeed, 0.0f, 0.5f);
	}
	ImGui::End();
}

void Camera::CameraUpdate(Mouse* mouse)
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotationY), DirectX::XMConvertToRadians(rotationX), DirectX::XMConvertToRadians(rotationZ));

	up = XMVector3TransformNormal(DirectX::XMVectorSet(0,1,0,0), rotationMatrix);
	right = XMVector3TransformNormal(DirectX::XMVectorSet(1, 0, 0, 0), rotationMatrix);
	forward = XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), rotationMatrix);
    // 기존 Position = Position + DirectX::XMVectorScale(forward,(mouse->GetWheelDelta() * mouseWheelSpeed));
    // DirectXMath의 XMVectorAdd를 사용하여 벡터 덧셈을 수행해야 합니다.
  
	if (mouse->GetMouseRight())
	{
		rotationX = rotationX - mouse->GetMouseDeltaPos().x * rotationSpeed;
		rotationY = rotationY + mouse->GetMouseDeltaPos().y * rotationSpeed;
		return;
	}
	Position = DirectX::XMVectorAdd(Position, DirectX::XMVectorScale(DirectX::XMVectorScale(forward, mouse->GetWheelDelta()), mouseWheelSpeed));
	if (!mouse->IsMouseWheelDown())
		return;
	Position = DirectX::XMVectorAdd(Position, DirectX::XMVectorScale(DirectX::XMVectorScale(right, mouse->GetMouseDeltaPos().x) , mouseSpeed));
	Position = DirectX::XMVectorAdd(Position, DirectX::XMVectorScale(DirectX::XMVectorScale(up, mouse->GetMouseDeltaPos().y), mouseSpeed));
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	return DirectX::XMMatrixInverse(NULL, DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotationY), DirectX::XMConvertToRadians(rotationX), DirectX::XMConvertToRadians(rotationZ)) * DirectX::XMMatrixTranslationFromVector(Position));
}

DirectX::XMFLOAT3 Camera::GetWorldPos()
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos,Position);
	return pos;
}

void Camera::KeyboardEvent(int idx)
{	
	if ((char)idx == 'W')
		Position = DirectX::XMVectorAdd(Position, DirectX::XMVectorScale(forward, mouseWheelSpeed));
	if ((char)idx == 'S')
		Position = DirectX::XMVectorAdd(Position, DirectX::XMVectorScale(forward, -mouseWheelSpeed));
	if ((char)idx == 'A')
		Position = DirectX::XMVectorAdd(Position, DirectX::XMVectorScale(right, -mouseWheelSpeed));
	if ((char)idx == 'D')
		Position = DirectX::XMVectorAdd(Position, DirectX::XMVectorScale(right, mouseWheelSpeed));
}
