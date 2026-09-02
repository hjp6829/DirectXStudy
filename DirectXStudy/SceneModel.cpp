#include "SceneModel.h"
#include "DirectXMain.h"
#include "ModelNode.h"
#include "Log.h"
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

void SceneModel::RenderModel(DirectXMain* dxdMain)
{
	XMMATRIX ViewMatrix = dxdMain->GetCamera()->GetViewMatrix();

	currentModelNode->RenderMeshs(dxdMain, worldMatrix);
	for (int i = 0; i < childNodes.size(); i++)
	{
		childNodes[i]->RenderModel(dxdMain);
	}
}

void SceneModel::UpdateModel()
{
	worldMatrix = XMMatrixScaling(modelScale.x, modelScale.y, modelScale.z) *
		XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(modelRot.x),
			XMConvertToRadians(modelRot.y),
			XMConvertToRadians(modelRot.z)
		) *
		XMMatrixTranslation(modelPos.x, modelPos.y, modelPos.z);
	currentModelNode->UpdateMeshs();
	for (int i = 0; i < childNodes.size(); i++)
	{
		childNodes[i]->UpdateModel();
	}
}

void SceneModel::SetMaterialIDX(int meshIDX, int MaterialIDX)
{
	//currentMeshs[meshIDX]->SetMaterialIDX(MaterialIDX);
}

void SceneModel::ToggleMeshEnable(bool value)
{
	meshEnable = value;
	currentModelNode->ToggleMeshEnable(meshEnable);
	for (int i = 0; i < childNodes.size(); i++)
	{
		childNodes[i]->ToggleMeshEnable(value);
	}
}
