#pragma once
#include "Bindable.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace Microsoft::WRL;
using namespace DirectX;

class VertexShader : public Bindable {
public:
	VertexShader(ID3D11Device* device, ID3DBlob* shaderBlob);
	void Bind(ID3D11DeviceContext* context) override;
private:
	ComPtr<ID3D11VertexShader> vs;
};