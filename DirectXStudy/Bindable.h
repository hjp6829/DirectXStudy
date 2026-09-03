#pragma once
#include <d3d11.h>
#include "d3dcompiler.h"
#include <vector>
#include "Object.h"


class Bindable
{
	public:
		Bindable(){}
		~Bindable() = default;
		virtual void Bind(ID3D11DeviceContext* context){}
};