#include "Sampler.h"

Sampler::Sampler(DirectXMain& dxdMain, int idx)
{
	this->index=idx;
	D3D11_SAMPLER_DESC sDc = {};
	sDc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sDc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sDc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sDc.BorderColor[0] = 0.0f;
	sDc.BorderColor[1] = 0.0f;
	sDc.BorderColor[2] = 0.0f;
	sDc.BorderColor[3] = 1.0f;

	GetDevice(dxdMain)->CreateSamplerState(&sDc, samplerState.GetAddressOf());
}

void Sampler::Bind(DirectXMain& dxdMain)
{
	GetContext(dxdMain)->PSSetSamplers(index,1,samplerState.GetAddressOf());
}
