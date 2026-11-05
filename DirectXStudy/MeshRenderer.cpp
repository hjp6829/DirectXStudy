#include "MeshRenderer.h"
#include "ModelNode.h"
#include "Object.h"
#include "imgui_impl_win32.h"
#include "EventDefinitions.h"
#include "EventBus.h"
#include "Datas.h"

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
		"localHesh",currentModelNode == nullptr ? 0 : currentModelNode->modelLocalHesh},
		{"ComponentName","MeshRenderer"},
		{"GUID",currentModelNode->GUID},
		{"localGUID",currentModelNode->GUID},
		{"MaterialGUID",material == nullptr ? "" : material->GUID}
	};

}

void MeshRenderer::Deserialize(const nlohmann::json& j)
{
	j.at("localHesh").get_to(localHesh);
	j.at("GUID").get_to(GUID);
	j.at("localGUID").get_to(GUID);
	std::string materialGUID;
	j.at("MaterialGUID").get_to(materialGUID);
	//assetmanager���� �ҷ��;���
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

void MeshRenderer::SetModelNode(ModelNode* node,  std::unordered_map<int, Material*>* materials)
{
	std::vector<int> materialIDXs = node->GetMeshsMaterialIDX();
	for(int idx : materialIDXs)
	{
		material.push_back(materials->at(idx));
	}
	node->SetMeshsMaterial(materials);
}

void MeshRenderer::ToggleMeshEnable(bool value)
{
	meshEnable = value;
	currentModelNode->ToggleMeshEnable(meshEnable);
}