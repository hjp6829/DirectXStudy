#pragma once
#include "Bindable.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class InputLayout : public Bindable
{
public:
	InputLayout(ID3D11Device* device, ID3DBlob* shaderBlob);
	void Bind(ID3D11DeviceContext* context) override;
	private:
		ComPtr<ID3D11InputLayout> pInputLayout;
};