#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>
#include "RenderingTypes.h"

class Mesh;

class ModelNode {
public:
	ModelNode(){}
	std::vector<Mesh*> currentMeshs;
	std::vector<ModelNode*> childNodes;
	std::string modelName;
	std::string sourceModelPath;
	DirectX::XMMATRIX localMatrix;
	uint64_t modelHeshCode;
public:
	void RenderMeshs(RenderPass renderPass, ID3D11DeviceContext* context, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, int objectID);
	void ToggleMeshEnable(bool value);
};