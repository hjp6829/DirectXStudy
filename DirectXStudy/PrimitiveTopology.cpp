#include "PrimitiveTopology.h"

PrimitiveTopology::PrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY TopologyType)
{
	type = TopologyType;
}

void PrimitiveTopology::Bind(DirectXMain& dxdMain)
{
	GetContext(dxdMain)->IASetPrimitiveTopology(type);
}
