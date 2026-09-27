#include "SceneObject.h"
#include "DirectXMain.h"
#include "ModelNode.h"
#include "Log.h"
XMFLOAT3 SceneObject::GetObjectPosition()
{
	XMFLOAT3 localPos;
	localPos.x = positionOffset.x + importedLocalPosition.x;
	localPos.y = positionOffset.y + importedLocalPosition.y;
	localPos.z = positionOffset.z + importedLocalPosition.z;
	return localPos;
}
XMFLOAT3 SceneObject::GetObjectRotation()
{
	XMFLOAT3 localRot;
	localRot.x = rotationOffset.x * importedLocalRotation.x;
	localRot.y = rotationOffset.y * importedLocalRotation.y;
	localRot.z = rotationOffset.z * importedLocalRotation.z;
	return localRot;
}
XMFLOAT3 SceneObject::GetObjectScale()
{
	XMFLOAT3 localScale;
	localScale.x = scaleOffset.x * importedLocalScale.x;
	localScale.y = scaleOffset.y * importedLocalScale.y;
	localScale.z = scaleOffset.z * importedLocalScale.z;
	return localScale;
}
void SceneObject::SetLocalTransform(XMFLOAT3 localPos, XMFLOAT3 localRot, XMFLOAT3 localScale)
{
	importedLocalPosition = localPos;
	importedLocalRotation = localRot;
	importedLocalScale = localScale;
}
void SceneObject::SetPosition(XMFLOAT3 position)
{
	positionOffset.x = position.x - importedLocalPosition.x;
	positionOffset.y = position.y - importedLocalPosition.y;
	positionOffset.z = position.z - importedLocalPosition.z;
}

void SceneObject::SetRotaion(XMFLOAT3 rotation)
{
	rotationOffset.x = rotation.x - importedLocalRotation.x;
	rotationOffset.y = rotation.y - importedLocalRotation.y;
	rotationOffset.z = rotation.z - importedLocalRotation.z;
}

void SceneObject::SetScale(XMFLOAT3 scale)
{
	scaleOffset.x = scale.x / importedLocalScale.x;
	scaleOffset.y = scale.y / importedLocalScale.y;
	scaleOffset.z = scale.z / importedLocalScale.z;
}

void SceneObject::RenderObject(DirectXMain* dxdMain)
{
	XMMATRIX ViewMatrix = dxdMain->GetCamera()->GetViewMatrix();
	XMMATRIX ProjectionMatrix = dxdMain->GetCamera()->GetProjectionMatrix();

	currentModelNode->RenderMeshs(dxdMain->GetContext(), worldMatrix, ViewMatrix, ProjectionMatrix);
	for (int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->RenderObject(dxdMain);
	}
}

void SceneObject::UpdateObject()
{
	worldMatrix = XMMatrixScaling(scaleOffset.x, scaleOffset.y, scaleOffset.z) *
		XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(rotationOffset.x),
			XMConvertToRadians(rotationOffset.y),
			XMConvertToRadians(rotationOffset.z)
		) *
		XMMatrixTranslation(positionOffset.x, positionOffset.y, positionOffset.z);
	if(parentObject != nullptr)
		worldMatrix = worldMatrix * parentObject->worldMatrix;
	currentModelNode->UpdateMeshs();
	for (int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->UpdateObject();
	}
}

void SceneObject::SetMaterialIDX(int meshIDX, int MaterialIDX)
{
	//currentMeshs[meshIDX]->SetMaterialIDX(MaterialIDX);
}

void SceneObject::ToggleMeshEnable(bool value)
{
	meshEnable = value;
	currentModelNode->ToggleMeshEnable(meshEnable);
	for (int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->ToggleMeshEnable(value);
	}
}

void SceneObject::RemoveModelData()
{
	if (parentObject)
	{
		parentObject->RemoveChildObject(this);
		parentObject =nullptr;
	}
	currentModelNode = nullptr;
	RemoveAllChileObject(this);
}

void SceneObject::RemoveChildObject(SceneObject* childObject)
{
	for (auto it = childObjects.begin(); it != childObjects.end(); ++it)
	{
		if (*it == childObject)
		{
			childObjects.erase(it);
			break;
		}
	}
}

void SceneObject::RemoveAllChileObject(SceneObject* childObject)
{
	for (int i = 0; i < childObject->childObjects.size(); i++)
	{
		RemoveAllChileObject(childObject->childObjects[i]);
		childObject->childObjects[i]->currentModelNode = nullptr;
	}
}

void SceneObject::InsertChildSceneObject(SceneObject* childObject)
{
	childObject->parentObject = this;
	childObjects.push_back(childObject);
}
