#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>
class Mesh;

class ModelNode {
public:
	std::vector<Mesh*> currentMeshs;
	std::vector<ModelNode*> childNodes;
	std::string modelName;
	void RenderMeshs(ID3D11DeviceContext* context, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
	void UpdateMeshs();
	void ToggleMeshEnable(bool value);
};