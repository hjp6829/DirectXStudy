#pragma once
#include "Bindable.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class PrimitiveTopology :public Bindable {
	public:
		PrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY TopologyType);
		void Bind(DirectXMain& dxdMain) override;
	private:
		D3D_PRIMITIVE_TOPOLOGY type;
};