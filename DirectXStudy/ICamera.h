#pragma once
#include <DirectXMath.h>

class ICamera
{
public:
    virtual ~ICamera() = default;

    virtual DirectX::XMMATRIX GetViewMatrix() const = 0;
    virtual DirectX::XMMATRIX GetProjectionMatrix() const = 0;

    virtual DirectX::XMFLOAT3 GetCameraPosition() const = 0;
    virtual DirectX::XMFLOAT3 GetCameraForward() const = 0;
};