#include "Bindable.h"

ID3D11Device* Bindable::GetDevice(DirectXMain& dxdMain)
{
	return dxdMain.pDevice.Get();
}

ID3D11DeviceContext* Bindable::GetContext(DirectXMain& dxdMain)
{
	return dxdMain.pContext.Get();
}