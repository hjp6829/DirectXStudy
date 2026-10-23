#pragma once
#include "ScriptComponent.h"

class TestScript : public ScriptComponent
{
public:
	TestScript(){};
	void Update() override;
	void Start() override;
	void InspectorUpdate() override;
	std::unique_ptr<Component> Clone()const override;
	void Serialize(nlohmann::json& j) override;
	void Deserialize(const nlohmann::json& j) override;
private:
	float moveSpeed = 5;
};