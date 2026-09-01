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
	ModelNode* currentModelNode;
	std::vector<SceneModel*> childNodes;
	XMFLOAT3 modelPos = { 0.0f, 0.0f, 4.0f };
	XMFLOAT3 modelRot;
	XMFLOAT3 modelScale = { 1.0f, 1.0f, 1.0f };
	XMMATRIX worldMatrix;
	void SetPosition(XMFLOAT3 position);
	void SetRotaion(XMFLOAT3 rotation);
	void SetScale(XMFLOAT3 scale);
	void RenderMeshs(DirectXMain* dxdMain);
	void UpdateMeshs();
	void SetMaterialIDX(int meshIDX,int MaterialIDX);
	void ToggleMeshEnable(bool value);
	bool meshEnable = true;
private:

};