#pragma once
#include "Object.h"

class AssimpConverter;

class Mesh : public Object {
public:
	Mesh(DirectXMain* dxdMain, ModelLoadData* modelLoadData, AssimpConverter* assimp, asMesh* meshData);
	~Mesh();
	void Update(XMMATRIX worldMatrix) override;
	void Render(DirectXMain* dxdMain) override;
	int GetMaterialIDX(){return materialIDX; }
	void SetMaterialIDX(asMaterial* material, int idx);
private:
	DirectXMain* dxdMain;
	Texture* meshTexture;
	Texture* normalTexture;
	int materialIDX;
};