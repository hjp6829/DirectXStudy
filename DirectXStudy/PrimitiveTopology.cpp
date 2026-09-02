#include "PrimitiveTopology.h"

PrimitiveTopology::PrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY TopologyType)
{
	type = TopologyType;
}

void PrimitiveTopology::Bind(ID3D11DeviceContext* context)
{
	context->IASetPrimitiveTopology(type);
}
