#include "SceneModel.h"
#include "DirectXMain.h"
#include "ModelNode.h"
#include "Log.h"
XMFLOAT3 SceneModel::GetModelPosition()
{
	XMFLOAT3 localPos;
	if(!test)
	{
		Log::PrintLog(modelName+std::to_string(positionOffset.x) + std::to_string(positionOffset.y) + std::to_string(positionOffset.z));
		test=true;
	}
	localPos.x = positionOffset.x + importedLocalPosition.x;
	localPos.y = positionOffset.y + importedLocalPosition.y;
	localPos.z = positionOffset.z + importedLocalPosition.z;
	return localPos;
}
void SceneModel::SetPosition(XMFLOAT3 position)
{
	positionOffset.x = position.x - importedLocalPosition.x;
	positionOffset.y = position.y - importedLocalPosition.y;
	positionOffset.z = position.z - importedLocalPosition.z;
}

void SceneModel::SetRotaion(XMFLOAT3 rotation)
{
	modelRot = rotation;
}

void SceneModel::SetScale(XMFLOAT3 scale)
{
	modelScale = scale;
}

void SceneModel::RenderModel(DirectXMain* dxdMain)
{
	XMMATRIX ViewMatrix = dxdMain->GetCamera()->GetViewMatrix();
	XMMATRIX ProjectionMatrix = dxdMain->GetCamera()->GetProjectionMatrix();

	currentModelNode->RenderMeshs(dxdMain->GetContext(), worldMatrix, ViewMatrix, ProjectionMatrix);
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
		XMMatrixTranslation(positionOffset.x, positionOffset.y, positionOffset.z);
	if(parentModel != nullptr)
		worldMatrix = worldMatrix * parentModel->worldMatrix;

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

void SceneModel::RemoveModelData()
{
	if (parentModel)
	{
		parentModel->RemoveChildModel(this);
		parentModel=nullptr;
	}
	currentModelNode = nullptr;
	RemoveAllChileModel(this);
}

void SceneModel::RemoveChildModel(SceneModel* childModel)
{
	for (auto it = childNodes.begin(); it != childNodes.end(); ++it)
	{
		if (*it == childModel)
		{
			childNodes.erase(it);
			break;
		}
	}
}

void SceneModel::RemoveAllChileModel(SceneModel* model)
{
	for (int i = 0; i < model->childNodes.size(); i++)
	{
		RemoveAllChileModel(model->childNodes[i]);
		model->childNodes[i]->currentModelNode = nullptr;
	}
}
