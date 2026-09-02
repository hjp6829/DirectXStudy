#include "VertexShader.h"

VertexShader::VertexShader(ID3D11Device* device, ID3DBlob* shaderBlob)
{
	device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, vs.GetAddressOf());
}

void VertexShader::Bind(ID3D11DeviceContext* context)
{
	context->VSSetShader(vs.Get(), 0, 0);
}


