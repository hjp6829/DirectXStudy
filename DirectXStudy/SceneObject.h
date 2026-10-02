#pragma once
#include "Object.h"

class SceneObject : public Object
{
public:
	SceneObject() {};
	void SetMaterialIDX(int meshIDX,int MaterialIDX);
	void Serialize(nlohmann::json& j) override;
private:
};