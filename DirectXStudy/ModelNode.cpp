#include "ModelNode.h"
#include "Mesh.h"

void ModelNode::RenderMeshs(RenderPass renderPass,ID3D11DeviceContext* context, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,int objectID)
{
	for (int i = 0; i < currentMeshs.size(); i++)
	{
		currentMeshs[i]->Render(renderPass,context, worldMatrix, viewMatrix, projectionMatrix, objectID);
	}
}

void ModelNode::ToggleMeshEnable(bool value)
{
	for (int i = 0; i < currentMeshs.size(); i++)
	{
		currentMeshs[i]->enable = value;
	}
}
