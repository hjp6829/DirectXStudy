#include "ModelNode.h"
#include "Mesh.h"

void ModelNode::RenderMeshs(ID3D11DeviceContext* context, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	for (int i = 0; i < currentMeshs.size(); i++)
	{
		currentMeshs[i]->Render(context, worldMatrix, viewMatrix, projectionMatrix);
	}
}

void ModelNode::OutlineRenderMeshs(ID3D11DeviceContext* context, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	for (int i = 0; i < currentMeshs.size(); i++)
	{
		currentMeshs[i]->OutlineRender(context, worldMatrix, viewMatrix, projectionMatrix);
	}
}

void ModelNode::ToggleMeshEnable(bool value)
{
	for (int i = 0; i < currentMeshs.size(); i++)
	{
		currentMeshs[i]->enable = value;
	}
}
