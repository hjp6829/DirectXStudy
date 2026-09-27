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
	SceneObject* parentObject;
	ModelNode* currentModelNode;
	std::vector<SceneObject*> childObjects;
	std::string objectName;
	std::string modelNamePath;
	uint64_t objectID;
	uint64_t parentobjectID;
	void RenderObject(DirectXMain* dxdMain);
	void UpdateObject();
	void SetMaterialIDX(int meshIDX,int MaterialIDX);
	void ToggleMeshEnable(bool value);
	bool meshEnable = true;
	bool IsRootObject() { return isRootObject; }
	void SetRootNodeCheck(bool value) { isRootObject = value; }
	void RemoveModelData();
	void RemoveChildObject(SceneObject* childObject);
	void RemoveAllChileObject(SceneObject* childObject);
	void InsertChildSceneObject(SceneObject* childObject);
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
	XMFLOAT3 GetObjectPosition();
	XMFLOAT3 GetObjectRotation();
	XMFLOAT3 GetObjectScale();
	void SetLocalTransform(XMFLOAT3 localPos, XMFLOAT3 localRot, XMFLOAT3 localScale);
private:
	XMFLOAT3 positionOffset = { 0.0f, 0.0f, 4.0f };
	XMFLOAT3 rotationOffset = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 scaleOffset = { 1.0f, 1.0f, 1.0f };
	XMMATRIX worldMatrix;
	XMFLOAT3 importedLocalPosition;
	XMFLOAT3 importedLocalRotation;
	XMFLOAT3 importedLocalScale;
	bool isRootObject;
};