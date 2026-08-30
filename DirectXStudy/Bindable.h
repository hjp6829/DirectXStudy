#pragma once
#include "DirectXMain.h"
class Bindable
{
	public:
		Bindable(){}
		~Bindable() = default;
		virtual void Bind(DirectXMain& dxdMain){}
protected:
	ID3D11Device* GetDevice(DirectXMain& dxdMain);
	ID3D11DeviceContext* GetContext(DirectXMain& dxdMain);
};