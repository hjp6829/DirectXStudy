#include "SceneModel.h"
#include "DirectXMain.h"
#include "Mesh.h"

void SceneModel::SetPosition(XMFLOAT3 position)
{
	modelPos = position;
	for (int i = 0; i < childNodes.size(); i++)
	{
		childNodes[i]->SetPosition(position);
	}
}

void SceneModel::SetRotaion(XMFLOAT3 rotation)
{
	modelRot = rotation;
	for (int i = 0; i < childNodes.size(); i++)
	{
		childNodes[i]->SetRotaion(rotation);
	}
}

void SceneModel::SetScale(XMFLOAT3 scale)
{
	modelScale = scale;
	for (int i = 0; i < childNodes.size(); i++)
	{
		childNodes[i]->SetScale(scale);
	}
}

void SceneModel::RenderMeshs(DirectXMain* dxdMain)
{
	XMMATRIX ViewMatrix = dxdMain->GetCamera()->GetViewMatrix();

	for (int i = 0; i < currentMeshs.size(); i++)
	{
		currentMeshs[i]->Render(dxdMain);
	}
}

void SceneModel::UpdateMeshs()
{
	worldMatrix = XMMatrixScaling(modelScale.x, modelScale.y, modelScale.z) *
		XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(modelRot.x),
			XMConvertToRadians(modelRot.y),
			XMConvertToRadians(modelRot.z)
		) *
		XMMatrixTranslation(modelPos.x, modelPos.y, modelPos.z);
	for (int j = 0; j < currentMeshs.size(); j++)
	{
		currentMeshs[j]->Update(worldMatrix);
	}
}

void SceneModel::SetMaterialIDX(int meshIDX, int MaterialIDX)
{
	//currentMeshs[meshIDX]->SetMaterialIDX(MaterialIDX);
}

void SceneModel::ToggleMeshEnable(bool value)
{
	meshEnable = value;
	for (int i = 0; i < currentMeshs.size(); i++)
	{
		currentMeshs[i]->enable = value;
	}
	for (int i = 0; i < childNodes.size(); i++)
	{
		childNodes[i]->ToggleMeshEnable(value);
	}
}
