#include "PixelShader.h"

PixelShader::PixelShader(DirectXMain& dxdMain, ID3DBlob* shaderBlob)
{
	GetDevice(dxdMain)->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, ps.GetAddressOf());
}

void PixelShader::Bind(DirectXMain& dxdMain)
{
	GetContext(dxdMain)->PSSetShader(ps.Get(), 0, 0);
}
