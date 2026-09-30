#pragma once
#include <DirectXMath.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <memory>
#include "Object.h"
#include "ModelNode.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class Mesh;

struct ConstantBufferData {
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 finalMatrix;
};
struct PSBuffer {
	XMFLOAT4 color;
};
struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
};

class ModelLoadData
{
public:
	ModelLoadData(){};
	std::string modelName;
	XMMATRIX mat;
	std::vector<int> meshIDX;
	aiNode* currentNode;
	std::vector<ModelLoadData*> childNodes;
	XMFLOAT3 localPos;
	XMFLOAT3 localRot;
	XMFLOAT3 localScale;
	//Model* currentModel;
	int idx;
	void Clear() {
		childNodes.clear();
	}
};

struct asMesh
{
	std::string meshName;
	aiMesh* origMesh;
	std::vector<Vertex> vertexs;
	std::vector<unsigned int> Indexs;
	unsigned int materialIDX;
};
struct asMaterial
{
	std::string name;
	ComPtr<ID3D11ShaderResourceView> textureView;
	ComPtr<ID3D11ShaderResourceView> normalView;
};
//struct SceneSaveData {
//	std::vector<JsonSceneObjectData> objectDatas;
//};
struct JsonSceneObjectData
{
	float localPosx, localPosy, localPosz;
	float localRotx, localRoty, localRotz;
	float localScalex, localScaley, localScalez;
	uint64_t modelHeshCode;
	uint64_t parentObjectID;
	uint64_t objectID;
	int isRootObject;
	std::string origModelPath;
	std::string testModelName;
	void SetTransformData(Object* object)
	{
		XMFLOAT3 positionOffset = object->GetPostionOffset();
		localPosx = positionOffset.x;
		localPosy = positionOffset.y;
		localPosz = positionOffset.z;

		XMFLOAT3 rotationOffset = object->GetRotationOffset();
		localRotx = rotationOffset.x;
		localRoty = rotationOffset.y;
		localRotz = rotationOffset.z;

		XMFLOAT3 scaleOffset = object->GetScaleOffset();
		localScalex = scaleOffset.x;
		localScaley = scaleOffset.y;
		localScalez = scaleOffset.z;

		modelHeshCode = object->currentModelNode->modelHeshCode;
	}
};