#include "MeshRenderer.h"
#include "ModelNode.h"

MeshRenderer::MeshRenderer()
{
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

void MeshRenderer::ToggleMeshEnable(bool value)
{
	meshEnable = value;
	currentModelNode->ToggleMeshEnable(meshEnable);
}