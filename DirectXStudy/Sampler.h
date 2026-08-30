#pragma once
#include "Bindable.h"
class Sampler : public Bindable
{
	public:
	Sampler(DirectXMain& dxdMain,int idx);
	void Bind(DirectXMain& dxdMain) override;
	private:
	ComPtr<ID3D11SamplerState> samplerState;
	int index;
};