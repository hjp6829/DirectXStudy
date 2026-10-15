#include "Transform.h"
#include "imgui_impl_win32.h"

Transform::Transform()
{
	UpdateDirectionVectors();
}

void Transform::SetPosition(DirectX::XMFLOAT3 position)
{
	positionOffset.x = position.x - importedLocalPosition.x;
	positionOffset.y = position.y - importedLocalPosition.y;
	positionOffset.z = position.z - importedLocalPosition.z;
}

void Transform::SetRotaion(DirectX::XMFLOAT3 rotation)
{
	rotationOffset.x = rotation.x - importedLocalRotation.x;
	rotationOffset.y = rotation.y - importedLocalRotation.y;
	rotationOffset.z = rotation.z - importedLocalRotation.z;
	UpdateDirectionVectors();
}

void Transform::SetScale(DirectX::XMFLOAT3 scale)
{
	scaleOffset.x = scale.x / importedLocalScale.x;
	scaleOffset.y = scale.y / importedLocalScale.y;
	scaleOffset.z = scale.z / importedLocalScale.z;
}

DirectX::XMFLOAT3 Transform::GetObjectPosition()
{
	DirectX::XMFLOAT3 localPos;
	localPos.x = positionOffset.x + importedLocalPosition.x;
	localPos.y = positionOffset.y + importedLocalPosition.y;
	localPos.z = positionOffset.z + importedLocalPosition.z;
	return localPos;
}
DirectX::XMFLOAT3 Transform::GetObjectRotation()
{
	DirectX::XMFLOAT3 localRot;
	localRot.x = rotationOffset.x * importedLocalRotation.x;
	localRot.y = rotationOffset.y * importedLocalRotation.y;
	localRot.z = rotationOffset.z * importedLocalRotation.z;
	return localRot;
}
DirectX::XMFLOAT3 Transform::GetObjectScale()
{
	DirectX::XMFLOAT3 localScale;
	localScale.x = scaleOffset.x * importedLocalScale.x;
	localScale.y = scaleOffset.y * importedLocalScale.y;
	localScale.z = scaleOffset.z * importedLocalScale.z;
	return localScale;
}

void Transform::SetRotationOffset(DirectX::XMFLOAT3 rotation)
{
	rotationOffset = rotation;
	UpdateDirectionVectors();
}

void Transform::MakeWorldMatrix()
{
	worldMatrix = DirectX::XMMatrixScaling(scaleOffset.x, scaleOffset.y, scaleOffset.z)*
		DirectX::XMMatrixRotationRollPitchYaw(
			DirectX::XMConvertToRadians(rotationOffset.x),
			DirectX::XMConvertToRadians(rotationOffset.y),
			DirectX::XMConvertToRadians(rotationOffset.z)
		)*
		DirectX::XMMatrixTranslation(positionOffset.x, positionOffset.y, positionOffset.z);
}

void Transform::InspectorUpdate()
{
	DirectX::XMFLOAT3 tempPos = GetObjectPosition();
	DirectX::XMFLOAT3 tempRot = GetObjectRotation();
	DirectX::XMFLOAT3 tempScale = GetObjectScale();

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
void Transform::UpdateDirectionVectors()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotationOffset.x), DirectX::XMConvertToRadians(rotationOffset.y), DirectX::XMConvertToRadians(rotationOffset.z));
	up = XMVector3TransformNormal(DirectX::XMVectorSet(0, 1, 0, 0), rotationMatrix);
	right = XMVector3TransformNormal(DirectX::XMVectorSet(1, 0, 0, 0), rotationMatrix);
	forward = XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), rotationMatrix);
}
void Transform::SetLocalTransform(DirectX::XMFLOAT3 localPos, DirectX::XMFLOAT3 localRot, DirectX::XMFLOAT3 localScale)
{
	importedLocalPosition = localPos;
	importedLocalRotation = localRot;
	importedLocalScale = localScale;
}
void Transform::Serialize(nlohmann::json& j)
{
	j = nlohmann::json{
		{"posX", positionOffset.x},
		{"posY", positionOffset.y},
		{"posZ", positionOffset.z},
		{ "rotx", rotationOffset.x },
		{"roty", rotationOffset.y},
		{"rotz", rotationOffset.z},
		{ "scalex", scaleOffset.x },
		{"scaley", scaleOffset.y},
		{"scalez", scaleOffset.z},
	};
}
void Transform::Deserialize(const nlohmann::json& j)
{
	const auto& transformJson = j.at("transform");
	SetPostionOffset(DirectX::XMFLOAT3(transformJson.at("posX").get<float>(), transformJson.at("posY").get<float>(), transformJson.at("posZ").get<float>()));
	SetRotationOffset(DirectX::XMFLOAT3(transformJson.at("rotx").get<float>(), transformJson.at("roty").get<float>(), transformJson.at("rotz").get<float>()));
	SetScaleOffset(DirectX::XMFLOAT3(transformJson.at("scalex").get<float>(), transformJson.at("scaley").get<float>(), transformJson.at("scalez").get<float>()));
}

void Transform::UpdateTransformForNewParent(Transform* parentTransform)
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR scale;
	DirectX::XMMATRIX inverseWorld = worldMatrix * DirectX::XMMatrixInverse(nullptr, parentTransform->GetWorldMatrix());

	XMMatrixDecompose(&scale, &rotation, &position, inverseWorld);
	XMStoreFloat3(&positionOffset, position);
	XMStoreFloat3(&rotationOffset, rotation);
	XMStoreFloat3(&scaleOffset, scale);
	UpdateDirectionVectors();
}
