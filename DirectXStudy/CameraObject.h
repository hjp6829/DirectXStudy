#pragma once
#include "Object.h"

class CameraObject : public Object {
public:
	DirectX::XMMATRIX GetProjectionMatrix();
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMFLOAT3 GetWorldPos();
	DirectX::XMFLOAT4 GetCameraPos()
	{
		DirectX::XMFLOAT4 pos(
			positionOffset.x,
			positionOffset.y,
			positionOffset.z,
			1.0f
		);
		return pos;
	}
	void SetMouseRightValue(bool value) { isMouseRight = value; }
	void SetMouseWheelDown(bool value) { isMouseWheelDown = value; }
	void SetMouseWheelDelta(int value);
	void SetMouseDeltaPos(int x,int y);
	void KeyboardEvent(int idx);
private:
	float FOV;
	float nearPlane = 0.5f;
	float farPlane = 1000.0f;
	DirectX::XMVECTOR up;
	DirectX::XMVECTOR right;
	DirectX::XMVECTOR forward;
	float rotationSpeed = 0.5f;
	float mouseSpeed = 0.03;
	float mouseWheelSpeed = 0.2;
	float KeyboardSpeed = 0.1;
	bool isMouseRight;
	bool isMouseWheelDown;
	int mouseWheelDelta;
};