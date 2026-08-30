#pragma once
#include "Object.h"

class AssimpConverter;

class LightObject : public Object
{
public:
	LightObject(DirectXMain* dxdMain, ModelLoadData* modelLoadData, AssimpConverter* assimp, asMesh* meshData);
	void Update(XMMATRIX worldMatrix) override;
	void Render(DirectXMain* dxdMain) override;
private:
	DirectXMain* dxdMain;
};