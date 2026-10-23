#pragma once
#include "Component.h"

class ModelNode;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	void Serialize(nlohmann::json& j) override;
	void Deserialize(const nlohmann::json& j) override;
	std::unique_ptr<Component> Clone()const override;
	void InspectorUpdate() override;
private:
	void ToggleMeshEnable(bool value);
private:
	ModelNode* currentModelNode;
	bool meshEnable = true;
};