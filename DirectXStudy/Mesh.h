#pragma once
#include "Object.h"

class AssimpConverter;
template<typename T>
class VertexConstantBufferContainer;
class ModelLoadData;
class AssimpConverter;
class asMesh;
class ID3D11Device;
class ID3D11DeviceContext;
class Texture;
class asMaterial;

class Mesh : public Object {
public:
	Mesh(ID3D11Device* device, ModelLoadData* modelLoadData, AssimpConverter* assimp, asMesh* meshData);
	~Mesh();
	void Update() override;
	void Render(ID3D11DeviceContext* context, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix) override;
	int GetMaterialIDX(){return materialIDX; }
	void SetMaterialIDX(asMaterial* material, int idx);
private:
	Texture* meshTexture;
	Texture* normalTexture;
	int materialIDX;
	VertexConstantBufferContainer<ConstantBufferData>* vertexConstantBufferContainer;
};