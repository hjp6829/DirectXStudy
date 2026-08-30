#include "IndexBuffer.h"

void IndexBuffer::Bind(DirectXMain& dxdMain)
{
	GetContext(dxdMain)->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

UINT IndexBuffer::GetIndexCount()
{
	return indexCount;
}
