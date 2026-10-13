#pragma once
#include "ScriptComponent.h"

class TestScript : public ScriptComponent
{
public:
	TestScript(){};
	void Update() override;
	void Start() override;
	int testid = 9123123;
};