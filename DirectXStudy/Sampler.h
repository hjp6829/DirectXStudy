#pragma once
#include "Bindable.h"
class Sampler : public Bindable
{
	public:
	Sampler(ID3D11Device* device,int idx);
	void Bind(ID3D11DeviceContext* context) override;
	private:
	ComPtr<ID3D11SamplerState> samplerState;
	int index;
};