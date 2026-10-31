#include "MeshRenderer.h"
#include "ModelNode.h"
#include "Object.h"
#include "imgui_impl_win32.h"
#include "EventDefinitions.h"
#include "EventBus.h"

MeshRenderer::MeshRenderer()
{	
	componentName = "MeshRenderer";
}

void MeshRenderer::Awake()
{

}

void MeshRenderer::RenderMesh(RenderPass renderPass, ID3D11DeviceContext* context, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	currentModelNode->RenderMeshs(renderPass, context, currentObject->GetTransform()->GetWorldMatrix(), viewMatrix, projectionMatrix, currentObject->objectID);
}

void MeshRenderer::Serialize(nlohmann::json& j)
{
	j = nlohmann::json{
	{
		"modelHeshCode",currentModelNode == nullptr ? 0 : currentModelNode->modelHeshCode},
		{"ComponentName","MeshRenderer"},
		{"modelPath",currentModelNode->sourceModelPath},
	};

}

void MeshRenderer::Deserialize(const nlohmann::json& j)
{
	j.at("modelHeshCode").get_to(modelNodeHesh);
	j.at("modelPath").get_to(modelPath);
}

std::unique_ptr<Component> MeshRenderer::Clone() const
{
	std::unique_ptr<MeshRenderer> Temp = std::make_unique<MeshRenderer>(*this);
	return std::move(Temp);
}

void MeshRenderer::InspectorUpdate()
{
	Component::InspectorUpdate();
	ImGui::Text(currentModelNode->modelName.c_str());
}

void MeshRenderer::ToggleMeshEnable(bool value)
{
	meshEnable = value;
	currentModelNode->ToggleMeshEnable(meshEnable);
}