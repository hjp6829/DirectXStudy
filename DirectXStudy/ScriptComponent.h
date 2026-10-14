#pragma once
#include "Component.h"

class ScriptComponent : public Component {
public:
	ScriptComponent(){};
	void Update() override;
	void Start() override;
	void InspectorUpdate() override;
};