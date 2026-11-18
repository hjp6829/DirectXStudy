#include "MeshRenderer.h"
#include "ModelNode.h"
#include "Object.h"
#include "imgui_impl_win32.h"
#include "EventDefinitions.h"
#include "EventBus.h"
#include "Datas.h"
#include "ModelAsset.h"

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
	};
	int slotCount = 0;
	for (auto& [materialIDX, slot] : materialSlots)
	{
		nlohmann::json materialJson;

		materialJson["slotIndex"] = slotCount++;
		materialJson["MaterialMode"] = slot.mode;
		if (slot.localMaterial != nullptr)
		{
			materialJson["localMaterial"] = materialIDX;
		}

		if (!slot.materialGUID.empty())
		{
			materialJson["materialGUID"] = slot.materialGUID;
		}

		j["MaterialSlots"].push_back(materialJson);
	}
}

void MeshRenderer::Deserialize(const nlohmann::json& j)
{
	j.at("localHesh").get_to(localHesh);
	j.at("GUID").get_to(GUID);
	std::string materialGUID;
	j.at("MaterialGUID").get_to(materialGUID);
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

	for(auto& slot : materialSlots)
	{
		if(slot.second.localMaterial != nullptr)
			ImGui::Text(slot.second.localMaterial->name.c_str());
		//if(slot.second.materialGUID != "")
		//	ImGui::Text(slot.second.localMaterial->name.c_str());
	}
}

void MeshRenderer::SetModelNode(ModelNode* node,  std::unordered_map<int, Material*>* materials)
{
	std::vector<int> materialIDXs = node->GetMeshsMaterialIDX();
	for(int idx : materialIDXs)
	{
		MaterialSlot slot;
		slot.localMaterial = materials->at(idx);
		materialSlots[idx] = slot;
	}
	node->SetMeshsMaterial(materials);
	currentModelNode = node;
}

void MeshRenderer::ResolveResources(const ComponentInitContext& context)
{
	MetaData* metaData = context.assetManager->GetMetaDataByGUID(GUID);
	ModelAsset* modelAsset = context.resourceManager->LoadModelAsset(metaData);
	ModelNode* modelNode = modelAsset->GetModelNode(localHesh);
	currentObject->GetTransform()->SetImportLocalMatrix(modelNode->localMatrix);
	SetModelNode(modelNode, modelAsset->GetMaterialsDic());
}

void MeshRenderer::ToggleMeshEnable(bool value)
{
	meshEnable = value;
	currentModelNode->ToggleMeshEnable(meshEnable);
}