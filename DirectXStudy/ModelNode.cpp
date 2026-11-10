#include "ModelNode.h"
#include "Mesh.h"
#include "Log.h"

std::vector<int> ModelNode::GetMeshsMaterialIDX()
{
    std::vector<int> result;
    std::unordered_set<int> visited;

    for (Mesh* mesh : currentMeshs)
    {
        if (mesh == nullptr)
            continue;

        int index = mesh->GetMaterialIDX();

        if (visited.insert(index).second)
        {
            result.push_back(index);
        }
    }

    return result;
}

void ModelNode::SetMeshsMaterial(std::unordered_map<int, Material*>* materialIDXs)
{
    for (Mesh* mesh : currentMeshs)
    {
        if (mesh == nullptr)
            continue;
        int index = mesh->GetMaterialIDX();
        material = materialIDXs->at(index);
        isMaterialChanged = true;
	}
}

void ModelNode::RenderMeshs(RenderPass renderPass,ID3D11DeviceContext* context, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,int objectID)
{
	for (int i = 0; i < currentMeshs.size(); i++)
	{
        if (isMaterialChanged)
        {
			currentMeshs[i]->SetMaterial(material);
        }
		currentMeshs[i]->Render(renderPass,context, worldMatrix, viewMatrix, projectionMatrix, objectID);
	}
    isMaterialChanged = false;
}

void ModelNode::ToggleMeshEnable(bool value)
{
	for (int i = 0; i < currentMeshs.size(); i++)
	{
		currentMeshs[i]->enable = value;
	}
}
