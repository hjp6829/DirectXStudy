#include "MeshRenderer.h"
#include "ModelNode.h"
#include "Object.h"
#include "imgui_impl_win32.h"

MeshRenderer::MeshRenderer()
{	
	componentName = "MeshRenderer";
}

void MeshRenderer::RenderMesh(RenderPass renderPass, ID3D11DeviceContext* context, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	currentModelNode->RenderMeshs(renderPass, context, currentObject->GetTransform()->GetWorldMatrix(), viewMatrix, projectionMatrix, currentObject->objectID);
}

void MeshRenderer::Serialize(nlohmann::json& j)
{
	j = nlohmann::json{
	{
		"modelHeshCode",currentModelNode == nullptr ? 0 : currentModelNode->modelHeshCode}
	};
}

void MeshRenderer::Deserialize(const nlohmann::json& j)
{
	
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