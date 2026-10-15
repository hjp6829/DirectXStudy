#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>
class Mesh;

class ModelNode {
public:
	ModelNode(){}
	std::vector<Mesh*> currentMeshs;
	std::vector<ModelNode*> childNodes;
	std::string modelName;
	std::string sourceModelPath;
	XMMATRIX localMatrix;
	uint64_t modelHeshCode;
public:
	void RenderMeshs(ID3D11DeviceContext* context, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
	void OutlineRenderMeshs(ID3D11DeviceContext* context, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
	void PickingRenderMeshs(ID3D11DeviceContext* context, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,int ObjectID);
	void ToggleMeshEnable(bool value);
};