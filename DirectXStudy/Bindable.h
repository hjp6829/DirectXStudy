#pragma once
#include "DirectXMain.h"
class Bindable
{
	public:
		Bindable(){}
		~Bindable() = default;
		virtual void Bind(ID3D11DeviceContext* context){}
};