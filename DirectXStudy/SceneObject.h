#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <vector>
#include <string>

class Mesh;
class DirectXMain;
class ModelNode;

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class SceneObject
{
public:
	SceneObject() {};
	SceneObject* parentModel;
	ModelNode* currentModelNode;
	std::vector<SceneObject*> childModels;
	std::string modelName;
	std::string modelNamePath;
	uint64_t modelID;
	uint64_t parentModelID;
	void RenderModel(DirectXMain* dxdMain);
	void UpdateModel();
	void SetMaterialIDX(int meshIDX,int MaterialIDX);
	void ToggleMeshEnable(bool value);
	bool meshEnable = true;
	bool IsRootModel() { return isRootNode; }
	void SetRootNodeCheck(bool value) { isRootNode  = value; }
	void RemoveModelData();
	void RemoveChildModel(SceneObject* childModel);
	void RemoveAllChileModel(SceneObject* childModel);
	void InsertChildSceneObject(SceneObject* childModel);
public:
	void SetPostionOffset(XMFLOAT3 position){ positionOffset = position;}
	void SetRotationOffset(XMFLOAT3 rotation) { rotationOffset = rotation; }
	void SetScaleOffset(XMFLOAT3 scale) { scaleOffset = scale; }
	XMFLOAT3 GetPostionOffset() { return positionOffset; }
	XMFLOAT3 GetRotationOffset() { return rotationOffset; }
	XMFLOAT3 GetScaleOffset() { return scaleOffset; }
	void SetPosition(XMFLOAT3 position);
	void SetRotaion(XMFLOAT3 rotation);
	void SetScale(XMFLOAT3 scale);
	XMFLOAT3 GetModelPosition();
	XMFLOAT3 GetModelRotation();
	XMFLOAT3 GetModelScale();
	void SetLocalTransform(XMFLOAT3 localPos, XMFLOAT3 localRot, XMFLOAT3 localScale);
private:
	XMFLOAT3 positionOffset = { 0.0f, 0.0f, 4.0f };
	XMFLOAT3 rotationOffset = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 scaleOffset = { 1.0f, 1.0f, 1.0f };
	XMMATRIX worldMatrix;
	XMFLOAT3 importedLocalPosition;
	XMFLOAT3 importedLocalRotation;
	XMFLOAT3 importedLocalScale;
	bool isRootNode;
};