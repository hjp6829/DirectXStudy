#pragma once
#include "Object.h"

class AssimpConverter;

class Mesh : public Object {
public:
	Mesh(ID3D11Device* device, ModelLoadData* modelLoadData, AssimpConverter* assimp, asMesh* meshData);
	~Mesh();
	void Update() override;
	void Render(ID3D11DeviceContext* context, XMMATRIX worldMatrix) override;
	int GetMaterialIDX(){return materialIDX; }
	void SetMaterialIDX(asMaterial* material, int idx);
private:
	DirectXMain* dxdMain;
	Texture* meshTexture;
	Texture* normalTexture;
	int materialIDX;
};