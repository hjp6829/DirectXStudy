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
	VertexShader(DirectXMain& dxdMain, ID3DBlob* shaderBlob);
	void Bind(DirectXMain& dxdMain) override;
private:
	ComPtr<ID3D11VertexShader> vs;
};