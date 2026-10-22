#include "Transform.h"
#include "imgui_impl_win32.h"
#include "Log.h"
#include "Component.h"
DirectX::XMFLOAT3 QuaternionToEulerDegrees(DirectX::FXMVECTOR quaternion);

Transform::Transform()
{
	componentName = "transform";
	UpdateDirectionVectors();
}

void Transform::SetPosition(DirectX::XMFLOAT3 position)
{
	DirectX::XMFLOAT3 importedPosition;
	DirectX::XMStoreFloat3(&importedPosition, importPosition);

	positionOffset.x = position.x - importedPosition.x;
	positionOffset.y = position.y - importedPosition.y;
	positionOffset.z = position.z - importedPosition.z;
}

void Transform::SetRotaion(DirectX::XMFLOAT3 rotation)
{
	using namespace DirectX;

	XMVECTOR targetQuaternion = XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(rotation.x),
		XMConvertToRadians(rotation.y),
		XMConvertToRadians(rotation.z)
	);//���Ϸ� -> ���ʹϾ�

	XMMATRIX offsetRotationMatrix =
		XMMatrixInverse(nullptr, XMMatrixRotationQuaternion(importRotation)) *
		XMMatrixRotationQuaternion(targetQuaternion);//������ ����� * ���°��� ��� 90 - 30 = 60
	//90�� ���� �� 30�� ����� 60�� offset ->���� ȸ������ +30��

	XMVECTOR offsetQuaternion = XMQuaternionRotationMatrix(offsetRotationMatrix);

	XMStoreFloat4(&rotationOffset,XMQuaternionNormalize(offsetQuaternion));

	UpdateDirectionVectors();
}

void Transform::SetScale(DirectX::XMFLOAT3 scale)
{
	DirectX::XMFLOAT3 importedScale;

	XMStoreFloat3(&importedScale, importScale);

	scaleOffset.x = scale.x / importedScale.x;
	scaleOffset.y = scale.y / importedScale.y;
	scaleOffset.z = scale.z / importedScale.z;
}

DirectX::XMFLOAT3 Transform::GetObjectPosition()
{
	DirectX::XMFLOAT3 importedPosition;
	XMStoreFloat3(&importedPosition, importPosition);

	return {
		importedPosition.x + positionOffset.x,
		importedPosition.y + positionOffset.y,
		importedPosition.z + positionOffset.z
	};
}
DirectX::XMFLOAT3 Transform::GetObjectRotation()
{
	using namespace DirectX;

	XMMATRIX rotationMatrix =
		XMMatrixRotationQuaternion(importRotation) *
		XMMatrixRotationQuaternion(XMLoadFloat4(&rotationOffset));

	XMVECTOR rotationQuaternion =
		XMQuaternionRotationMatrix(rotationMatrix);

	return QuaternionToEulerDegrees(rotationQuaternion);
}
DirectX::XMFLOAT3 Transform::GetObjectScale()
{
	DirectX::XMFLOAT3 importedScale;

	DirectX::XMStoreFloat3(&importedScale,importScale);
	return {
		importedScale.x * scaleOffset.x,
		importedScale.y * scaleOffset.y,
		importedScale.z * scaleOffset.z
	};
}

void Transform::SetRotationOffset(DirectX::XMFLOAT4 rotation)
{
	using namespace DirectX;

	rotationOffset = rotation;

	UpdateDirectionVectors();
}

void Transform::MakeLocalMatrix()
{
	DirectX::XMVECTOR quaternion = XMLoadFloat4(&rotationOffset);
	offsetMatrix =
		DirectX::XMMatrixScaling(scaleOffset.x, scaleOffset.y, scaleOffset.z) *
		DirectX::XMMatrixRotationQuaternion(quaternion) *//���ʹϾ��� xmmatrix�� ��ȯ
		DirectX::XMMatrixTranslation(
			positionOffset.x,
			positionOffset.y,
			positionOffset.z
		);
	localMatrix = importLocalMatrix * offsetMatrix ;
}

void Transform::CopyTransform(Transform* origTransform)
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR scale;

	DirectX::XMMATRIX matrixTemp = origTransform->GetOffsetMatrix();
	DirectX::XMMatrixDecompose(&scale,&rotation,&position, matrixTemp);

	XMStoreFloat3(&positionOffset, position);
	XMStoreFloat4(&rotationOffset, rotation);
	XMStoreFloat3(&scaleOffset, scale);

	SetImportLocalMatrix(origTransform->GetImportLocalMatrix());
}

std::unique_ptr<Component> Transform::Clone() const
{
	std::unique_ptr<Transform> Temp = std::make_unique<Transform>(*this);
	Temp->componentName = "CopyTransform";
	return std::move(Temp);
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
	using namespace DirectX;

	XMMATRIX rotationMatrix =
		XMMatrixRotationQuaternion(importRotation) *
		XMMatrixRotationQuaternion(XMLoadFloat4(&rotationOffset));

	up = XMVector3Normalize(
		XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), rotationMatrix));

	right = XMVector3Normalize(
		XMVector3TransformNormal(XMVectorSet(1, 0, 0, 0), rotationMatrix));

	forward = XMVector3Normalize(
		XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), rotationMatrix));
}
void Transform::SetImportLocalMatrix(DirectX::XMMATRIX localMatrix)
{
	this->importLocalMatrix = localMatrix;
	DirectX::XMMatrixDecompose(&importScale, &importRotation, &importPosition, importLocalMatrix);
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
		{"rotw", rotationOffset.w},
		{ "scalex", scaleOffset.x },
		{"scaley", scaleOffset.y},
		{"scalez", scaleOffset.z},
	};
}
void Transform::Deserialize(const nlohmann::json& j)
{
	const auto& transformJson = j.at("transform");
	SetPostionOffset(DirectX::XMFLOAT3(transformJson.at("posX").get<float>(), transformJson.at("posY").get<float>(), transformJson.at("posZ").get<float>()));
	SetRotationOffset(DirectX::XMFLOAT4(transformJson.at("rotx").get<float>(), transformJson.at("roty").get<float>(), transformJson.at("rotz").get<float>(), transformJson.at("rotw").get<float>()));
	SetScaleOffset(DirectX::XMFLOAT3(transformJson.at("scalex").get<float>(), transformJson.at("scaley").get<float>(), transformJson.at("scalez").get<float>()));
}

void Transform::UpdateTransformForNewParent(DirectX::FXMMATRIX oldWorldMatrix, DirectX::FXMMATRIX newParentWorldMatrix)
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR scale;
	DirectX::XMMATRIX inverseLocal = oldWorldMatrix * DirectX::XMMatrixInverse(nullptr, newParentWorldMatrix);

	XMMatrixDecompose(&scale, &rotation, &position, inverseLocal);
	XMStoreFloat3(&positionOffset, position);
	XMStoreFloat4(&rotationOffset, rotation);
	XMStoreFloat3(&scaleOffset, scale);
	UpdateDirectionVectors();
}
DirectX::XMFLOAT3 QuaternionToEulerDegrees(DirectX::FXMVECTOR quaternion)
{
	using namespace DirectX;

	XMFLOAT4 q;
	XMStoreFloat4(&q, XMQuaternionNormalize(quaternion));

	// X�� ȸ�� (Pitch)
	float sinPitch = 2.0f * (q.w * q.x - q.y * q.z);
	sinPitch = std::clamp(sinPitch, -1.0f, 1.0f);

	float pitch = std::asin(sinPitch);

	// Y�� ȸ�� (Yaw)
	float yaw = std::atan2(
		2.0f * (q.w * q.y + q.x * q.z),
		1.0f - 2.0f * (q.x * q.x + q.y * q.y)
	);

	// Z�� ȸ�� (Roll)
	float roll = std::atan2(
		2.0f * (q.w * q.z + q.x * q.y),
		1.0f - 2.0f * (q.x * q.x + q.z * q.z)
	);

	return XMFLOAT3(
		XMConvertToDegrees(pitch),
		XMConvertToDegrees(yaw),
		XMConvertToDegrees(roll)
	);
}