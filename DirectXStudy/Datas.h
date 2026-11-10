#pragma once
#include <DirectXMath.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <memory>
#include <filesystem>

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class Mesh;
class Object;
class ModelNode;
struct asMesh;

struct ConstantBufferData {
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 finalMatrix;
};
struct PSBuffer {
	XMFLOAT4 color;
	float isTexture;
	float isNormal;
	float padding[2];
};
struct PSPickingIDBuffer {
	int id;
	float padding[3];
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
	std::vector<asMesh*> meshs;
	aiNode* currentNode;
	std::vector<ModelLoadData*> childNodes;
	XMMATRIX localMatrix;
	std::string localPath;
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
struct TextureAsset {
	ComPtr<ID3D11Texture2D> texture;
	ComPtr<ID3D11ShaderResourceView> srv;
};
struct Material
{
	std::string name;
	TextureAsset* texture = nullptr;
	TextureAsset* normal = nullptr;
};

struct MeshData {
	std::vector<Vertex> vertexs;
	std::vector<unsigned int> Indexs;
	Material* material = nullptr;
};

enum class AssetType {
	Unknown,
	TextureAssetType,
	ModelAssetType,
	MaterialAssetType,
	FolderAssetType,
};

struct MetaData {
	std::string GUID;
	AssetType assetType;
	std::filesystem::path assetPath;
};

struct MaterialData {
	std::string GUID;
	std::string textureGUID;
	std::string normalGUID;
	std::string name;
};
struct TextureRequest
{
	size_t materialIndex;
	aiTextureType type;
	std::filesystem::path path;
};
