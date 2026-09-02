#pragma once
#include "Object.h"

class AssimpConverter;

class LightObject : public Object
{
public:
	LightObject(DirectXMain* dxdMain, ModelLoadData* modelLoadData, AssimpConverter* assimp, asMesh* meshData);
	void Update() override;
	void Render(DirectXMain* dxdMain, XMMATRIX worldMatrix) override;
private:
	DirectXMain* dxdMain;
};