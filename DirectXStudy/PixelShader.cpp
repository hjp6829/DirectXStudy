#include "PixelShader.h"

PixelShader::PixelShader(ID3D11Device* device, ID3DBlob* shaderBlob)
{
	device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, ps.GetAddressOf());
}

void PixelShader::Bind(ID3D11DeviceContext* context)
{
	context->PSSetShader(ps.Get(), 0, 0);
}
