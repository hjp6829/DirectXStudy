#include "Transform.h"
#include "imgui_impl_win32.h"
DirectX::XMFLOAT3 QuaternionToEulerDegrees(DirectX::FXMVECTOR quaternion);

Transform::Transform()
{
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
	);//오일러 -> 쿼터니언

	XMMATRIX offsetRotationMatrix =
		XMMatrixInverse(nullptr, XMMatrixRotationQuaternion(importRotation)) *
		XMMatrixRotationQuaternion(targetQuaternion);//원본의 역행렬 * 들어온값의 행렬 90 - 30 = 60
	//90은 들어온 값 30은 역행렬 60은 offset ->원본 회전값은 +30임

	XMVECTOR offsetQuaternion = XMQuaternionRotationMatrix(offsetRotationMatrix);

	XMStoreFloat4(&rotationOffset,XMQuaternionNormalize(offsetQuaternion));
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

	XMMATRIX matrix = importLocalMatrix *XMMatrixRotationQuaternion(XMLoadFloat4(&rotationOffset));

	XMVECTOR scale;
	XMVECTOR rotation;
	XMVECTOR position;

	if (!XMMatrixDecompose(&scale, &rotation, &position, matrix))
		return { 0, 0, 0 };

	return QuaternionToEulerDegrees(rotation);
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

void Transform::SetRotationOffset(DirectX::XMFLOAT3 rotation)
{
	using namespace DirectX;

	XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(rotation.x),
		XMConvertToRadians(rotation.y),
		XMConvertToRadians(rotation.z)
	);

	XMStoreFloat4(&rotationOffset, quaternion);
	UpdateDirectionVectors();
}

void Transform::MakeLocalMatrix()
{
	DirectX::XMVECTOR quaternion = XMLoadFloat4(&rotationOffset);
	DirectX::XMMATRIX offsetMatrix =
		DirectX::XMMatrixScaling(scaleOffset.x, scaleOffset.y, scaleOffset.z) *
		DirectX::XMMatrixRotationQuaternion(quaternion) *//쿼터니언을 xmmatrix로 변환
		DirectX::XMMatrixTranslation(
			positionOffset.x,
			positionOffset.y,
			positionOffset.z
		);
	localMatrix = importLocalMatrix * offsetMatrix ;
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
void Transform::SetImportLocalTransform(DirectX::XMMATRIX localMatrix)
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

	// X축 회전 (Pitch)
	float sinPitch = 2.0f * (q.w * q.x - q.y * q.z);
	sinPitch = std::clamp(sinPitch, -1.0f, 1.0f);

	float pitch = std::asin(sinPitch);

	// Y축 회전 (Yaw)
	float yaw = std::atan2(
		2.0f * (q.w * q.y + q.x * q.z),
		1.0f - 2.0f * (q.x * q.x + q.y * q.y)
	);

	// Z축 회전 (Roll)
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