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

class SceneModel
{
public:
	SceneModel() {};
	SceneModel* parentModel;
	ModelNode* currentModelNode;
	std::vector<SceneModel*> childNodes;
	std::string modelName;
	XMFLOAT3 modelRot;
	XMFLOAT3 modelScale = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 importedLocalPosition;
	XMMATRIX worldMatrix;
	XMFLOAT3 GetModelPosition();
	void SetPosition(XMFLOAT3 position);
	void SetRotaion(XMFLOAT3 rotation);
	void SetScale(XMFLOAT3 scale);
	void RenderModel(DirectXMain* dxdMain);
	void UpdateModel();
	void SetMaterialIDX(int meshIDX,int MaterialIDX);
	void ToggleMeshEnable(bool value);
	bool meshEnable = true;
	bool IsRootModel() { return parentModel == nullptr; }
	void RemoveModelData();
	void RemoveChildModel(SceneModel* childModel);
	void RemoveAllChileModel(SceneModel* childModel);
	bool test;
private:
	XMFLOAT3 positionOffset = { 0.0f, 0.0f, 4.0f };
};