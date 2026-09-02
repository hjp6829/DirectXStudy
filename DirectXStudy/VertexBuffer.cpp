#include "VertexBuffer.h"
void VertexBuffer::Bind(ID3D11DeviceContext* context)
{
	const UINT offset = 0;
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
}
