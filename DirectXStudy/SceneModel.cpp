#include "SceneModel.h"
#include "DirectXMain.h"
#include "ModelNode.h"
#include "Log.h"
XMFLOAT3 SceneModel::GetModelPosition()
{
	XMFLOAT3 localPos;
	localPos.x = positionOffset.x + importedLocalPosition.x;
	localPos.y = positionOffset.y + importedLocalPosition.y;
	localPos.z = positionOffset.z + importedLocalPosition.z;
	return localPos;
}
XMFLOAT3 SceneModel::GetModelRotation()
{
	XMFLOAT3 localRot;
	localRot.x = rotationOffset.x * importedLocalRotation.x;
	localRot.y = rotationOffset.y * importedLocalRotation.y;
	localRot.z = rotationOffset.z * importedLocalRotation.z;
	return localRot;
}
XMFLOAT3 SceneModel::GetModelScale()
{
	XMFLOAT3 localScale;
	localScale.x = scaleOffset.x * importedLocalScale.x;
	localScale.y = scaleOffset.y * importedLocalScale.y;
	localScale.z = scaleOffset.z * importedLocalScale.z;
	return localScale;
}
void SceneModel::SetLocalTransform(XMFLOAT3 localPos, XMFLOAT3 localRot, XMFLOAT3 localScale)
{
	importedLocalPosition = localPos;
	importedLocalRotation = localRot;
	importedLocalScale = localScale;
}
void SceneModel::SetPosition(XMFLOAT3 position)
{
	positionOffset.x = position.x - importedLocalPosition.x;
	positionOffset.y = position.y - importedLocalPosition.y;
	positionOffset.z = position.z - importedLocalPosition.z;
}

void SceneModel::SetRotaion(XMFLOAT3 rotation)
{
	rotationOffset.x = rotation.x - importedLocalRotation.x;
	rotationOffset.y = rotation.y - importedLocalRotation.y;
	rotationOffset.z = rotation.z - importedLocalRotation.z;
}

void SceneModel::SetScale(XMFLOAT3 scale)
{
	scaleOffset.x = scale.x / importedLocalScale.x;
	scaleOffset.y = scale.y / importedLocalScale.y;
	scaleOffset.z = scale.z / importedLocalScale.z;
}

void SceneModel::RenderModel(DirectXMain* dxdMain)
{
	XMMATRIX ViewMatrix = dxdMain->GetCamera()->GetViewMatrix();
	XMMATRIX ProjectionMatrix = dxdMain->GetCamera()->GetProjectionMatrix();

	currentModelNode->RenderMeshs(dxdMain->GetContext(), worldMatrix, ViewMatrix, ProjectionMatrix);
	for (int i = 0; i < childModels.size(); i++)
	{
		childModels[i]->RenderModel(dxdMain);
	}
}

void SceneModel::UpdateModel()
{
	worldMatrix = XMMatrixScaling(scaleOffset.x, scaleOffset.y, scaleOffset.z) *
		XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(rotationOffset.x),
			XMConvertToRadians(rotationOffset.y),
			XMConvertToRadians(rotationOffset.z)
		) *
		XMMatrixTranslation(positionOffset.x, positionOffset.y, positionOffset.z);
	if(parentModel != nullptr)
		worldMatrix = worldMatrix * parentModel->worldMatrix;

	currentModelNode->UpdateMeshs();
	for (int i = 0; i < childModels.size(); i++)
	{
		childModels[i]->UpdateModel();
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
	for (int i = 0; i < childModels.size(); i++)
	{
		childModels[i]->ToggleMeshEnable(value);
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
	for (auto it = childModels.begin(); it != childModels.end(); ++it)
	{
		if (*it == childModel)
		{
			childModels.erase(it);
			break;
		}
	}
}

void SceneModel::RemoveAllChileModel(SceneModel* model)
{
	for (int i = 0; i < model->childModels.size(); i++)
	{
		RemoveAllChileModel(model->childModels[i]);
		model->childModels[i]->currentModelNode = nullptr;
	}
}

void SceneModel::InsertChildSceneModel(SceneModel* childModel)
{
	childModel->parentModel = this;
	childModels.push_back(childModel);
}
