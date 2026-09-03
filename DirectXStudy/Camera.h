#pragma once
#include <DirectXMath.h>
#include <Windows.h>

class Mouse;
class Keyboard;

class Camera {
public:
	Camera(Keyboard* keyboard);
	void ViewCameraWindow();
	void CameraUpdate(Mouse* mouse);
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();
	DirectX::XMFLOAT3 GetWorldPos();
	DirectX::XMFLOAT4 GetCameraPos() 
	{ 
		DirectX::XMFLOAT4 pos;
		DirectX::XMStoreFloat4(&pos, Position);
		return pos;
	}
private:
	DirectX::XMVECTOR Position = DirectX::XMVectorSet(0,0,0,1);
	float rotationX = 0, rotationY = 0, rotationZ=0;
	float mouseSpeed = 0.03;
	float mouseWheelSpeed = 0.2;
	float KeyboardSpeed = 0.1;
	float rotationSpeed = 0.5f;
	DirectX::XMVECTOR up;
	DirectX::XMVECTOR right;
	DirectX::XMVECTOR forward;
private:
	void KeyboardEvent(int idx);
};