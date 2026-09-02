#pragma once
#include <vector>
#include <string>

class Mesh;

class ModelNode {
public:
	std::vector<Mesh*> currentMeshs;
	std::vector<ModelNode*> childNodes;
	std::string modelName;
	void RenderMeshs(ID3D11DeviceContext* context, DirectX::XMMATRIX worldMatrix);
	void UpdateMeshs();
	void ToggleMeshEnable(bool value);
};