#include "VertexBuffer.h"
void VertexBuffer::Bind(DirectXMain& dxdMain)
{
	const UINT offset = 0;
	GetContext(dxdMain)->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
}
