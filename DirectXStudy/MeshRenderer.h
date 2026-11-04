#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "RenderingTypes.h"
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

class ModelNode;
struct Material;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	void Awake() override;
	void RenderMesh(RenderPass renderPass, ID3D11DeviceContext* context, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
	void Serialize(nlohmann::json& j) override;
	void Deserialize(const nlohmann::json& j) override;
	std::unique_ptr<Component> Clone()const override;
	void InspectorUpdate() override;
	void SetModelNode(ModelNode* node)
	{ 
		currentModelNode = node;
	}
	uint64_t GetLocalHesh() {return localHesh; }
	std::string GetModelGUID() { return GUID; }
private:
	void ToggleMeshEnable(bool value);
private:
	ModelNode* currentModelNode;
	uint64_t localHesh;
	std::string GUID;
	bool meshEnable = true;
	Material* material;
};