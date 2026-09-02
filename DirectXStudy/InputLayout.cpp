#include "InputLayout.h"

InputLayout::InputLayout(ID3D11Device* device, ID3DBlob* shaderBlob)
{
	D3D11_INPUT_ELEMENT_DESC iDec[] = {
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"UV",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,20,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"Tangent",0,DXGI_FORMAT_R32G32B32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	device->CreateInputLayout(iDec, (UINT)_countof(iDec), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), pInputLayout.GetAddressOf());
}

void InputLayout::Bind(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(pInputLayout.Get());
}

