#include "IndexBuffer.h"

void IndexBuffer::Bind(ID3D11DeviceContext* context)
{
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

UINT IndexBuffer::GetIndexCount()
{
	return indexCount;
}
