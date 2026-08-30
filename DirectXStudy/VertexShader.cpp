#include "VertexShader.h"

VertexShader::VertexShader(DirectXMain& dxdMain, ID3DBlob* shaderBlob)
{
	GetDevice(dxdMain)->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, vs.GetAddressOf());
}

void VertexShader::Bind(DirectXMain& dxdMain)
{
	GetContext(dxdMain)->VSSetShader(vs.Get(), 0, 0);
}


