#include "Sampler.h"

Sampler::Sampler(ID3D11Device* device, int idx)
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

	device->CreateSamplerState(&sDc, samplerState.GetAddressOf());
}

void Sampler::Bind(ID3D11DeviceContext* context)
{
	context->PSSetSamplers(index,1,samplerState.GetAddressOf());
}
