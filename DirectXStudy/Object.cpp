#include "Object.h"
#include "DirectXMain.h"
#include "ModelNode.h"
#include "Log.h"
#include "imgui_impl_win32.h"
XMFLOAT3 Object::GetObjectPosition()
{
	XMFLOAT3 localPos;
	localPos.x = positionOffset.x + importedLocalPosition.x;
	localPos.y = positionOffset.y + importedLocalPosition.y;
	localPos.z = positionOffset.z + importedLocalPosition.z;
	return localPos;
}
XMFLOAT3 Object::GetObjectRotation()
{
	XMFLOAT3 localRot;
	localRot.x = rotationOffset.x * importedLocalRotation.x;
	localRot.y = rotationOffset.y * importedLocalRotation.y;
	localRot.z = rotationOffset.z * importedLocalRotation.z;
	return localRot;
}
XMFLOAT3 Object::GetObjectScale()
{
	XMFLOAT3 localScale;
	localScale.x = scaleOffset.x * importedLocalScale.x;
	localScale.y = scaleOffset.y * importedLocalScale.y;
	localScale.z = scaleOffset.z * importedLocalScale.z;
	return localScale;
}
void Object::SetLocalTransform(XMFLOAT3 localPos, XMFLOAT3 localRot, XMFLOAT3 localScale)
{
	importedLocalPosition = localPos;
	importedLocalRotation = localRot;
	importedLocalScale = localScale;
}
void Object::ShowInspectorUI()
{
	if (ImGui::Begin("Insfector"))
	{
		DrawInspectorContents();
	}
	ImGui::End();
}
void Object::DrawInspectorContents()
{
	XMFLOAT3 tempPos = GetObjectPosition();
	XMFLOAT3 tempRot = GetObjectRotation();
	XMFLOAT3 tempScale = GetObjectScale();

	bool enableValue = meshEnable;
	ImGui::Checkbox("Enable", &enableValue);
	if (enableValue != meshEnable)
		ToggleMeshEnable(enableValue);
	//ImGui::Text(currentSceneObject->modelName.c_str());
	ImGui::Text("Psotion");
	if (ImGui::InputFloat3("Position", &tempPos.x))
		SetPosition(tempPos);
	ImGui::Text("Rotation");
	if (ImGui::InputFloat3("Rotation", &tempRot.x))
		SetRotaion(tempRot);
	ImGui::Text("Scale");
	if (ImGui::InputFloat3("Scale", &tempScale.x))
		SetScale(tempScale);
}
void Object::Serialize(nlohmann::json& j)
{
	j = nlohmann::json{
		{"modelHeshCode",currentModelNode == nullptr ? 0 : currentModelNode->modelHeshCode},
		{"parentObjectID", parentObject == nullptr ? 0 : parentObject->objectID},
		{"posX", positionOffset.x},
		{"posY", positionOffset.y},
		{"posZ", positionOffset.z},
		{ "rotx", rotationOffset.x },
		{"roty", rotationOffset.y},
		{"rotz", rotationOffset.z},
		{ "scalex", scaleOffset.x },
		{"scaley", scaleOffset.y},
		{"scalez", scaleOffset.z},
		{"ModelNamePath", modelNamePath},
		{ "TestModelNamePath", objectName},
		{ "objectID", objectID },
		{ "isRootObject", IsRootObject() == true ? 1 : 0}
	};
}
void Object::Deserialize(const nlohmann::json& j)
{
	parentobjectID = j["parentObjectID"];
	SetPostionOffset(XMFLOAT3(j["posX"], j["posY"], j["posZ"]));
	SetRotationOffset(XMFLOAT3(j["rotx"], j["roty"], j["rotz"]));
	SetScaleOffset(XMFLOAT3(j["scalex"], j["scaley"], j["scalez"]));
	objectID = j["objectID"];
	SetRootNodeCheck(j["isRootObject"] == 1 ? true : false);
}
void Object::SetPosition(XMFLOAT3 position)
{
	positionOffset.x = position.x - importedLocalPosition.x;
	positionOffset.y = position.y - importedLocalPosition.y;
	positionOffset.z = position.z - importedLocalPosition.z;
}

void Object::SetRotaion(XMFLOAT3 rotation)
{
	rotationOffset.x = rotation.x - importedLocalRotation.x;
	rotationOffset.y = rotation.y - importedLocalRotation.y;
	rotationOffset.z = rotation.z - importedLocalRotation.z;
}

void Object::SetScale(XMFLOAT3 scale)
{
	scaleOffset.x = scale.x / importedLocalScale.x;
	scaleOffset.y = scale.y / importedLocalScale.y;
	scaleOffset.z = scale.z / importedLocalScale.z;
}

void Object::RenderObject(DirectXMain* dxdMain)
{
	XMMATRIX ViewMatrix = dxdMain->GetCamera()->GetViewMatrix();
	XMMATRIX ProjectionMatrix = dxdMain->GetCamera()->GetProjectionMatrix();

	if(currentModelNode != nullptr)
		currentModelNode->RenderMeshs(dxdMain->GetContext(), worldMatrix, ViewMatrix, ProjectionMatrix);
	for (int i = 0; i < childObjects.size(); i++)
	{
		if (childObjects[i] != nullptr)
			childObjects[i]->RenderObject(dxdMain);
	}
}

void Object::UpdateObject()
{
	worldMatrix = XMMatrixScaling(scaleOffset.x, scaleOffset.y, scaleOffset.z) *
		XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(rotationOffset.x),
			XMConvertToRadians(rotationOffset.y),
			XMConvertToRadians(rotationOffset.z)
		) *
		XMMatrixTranslation(positionOffset.x, positionOffset.y, positionOffset.z);
	if (parentObject != nullptr)
		worldMatrix = worldMatrix * parentObject->worldMatrix;
	for (int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->UpdateObject();
	}
}

void Object::ToggleMeshEnable(bool value)
{
	meshEnable = value;
	currentModelNode->ToggleMeshEnable(meshEnable);
	for (int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->ToggleMeshEnable(value);
	}
}

void Object::RemoveModelData()
{
	if (parentObject)
	{
		parentObject->RemoveChildObject(this);
		parentObject = nullptr;
	}
	currentModelNode = nullptr;
	RemoveAllChileObject(this);
}

void Object::RemoveChildObject(Object* childObject)
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

void Object::RemoveAllChileObject(Object* childObject)
{
	for (int i = 0; i < childObject->childObjects.size(); i++)
	{
		RemoveAllChileObject(childObject->childObjects[i]);
		childObject->childObjects[i]->currentModelNode = nullptr;
	}
}

void Object::InsertChildSceneObject(Object* childObject)
{
	childObject->parentObject = this;
	childObjects.push_back(childObject);
}

void Object::UpdateTransformForNewParent(Object* parentObject)
{
	XMVECTOR position;
	XMVECTOR rotation;
	XMVECTOR scale;
	XMMATRIX inverseWorld = worldMatrix * XMMatrixInverse(nullptr, parentObject->worldMatrix);

	XMMatrixDecompose(&scale, &rotation, &position, inverseWorld);
	XMStoreFloat3(&positionOffset, position);
	XMStoreFloat3(&rotationOffset, rotation);
	XMStoreFloat3(&scaleOffset, scale);
}
