#pragma once
#include <DirectXMath.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <memory>
#include <filesystem>
#include "AssetManager.h"
#include "ResourceManager.h"

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

struct MeshLoadData {
	std::vector<Vertex> vertexs;
	std::vector<unsigned int> Indexs;
	unsigned int materialIDX;
};

class ModelLoadData
{
public:
	ModelLoadData(){};
	std::string modelName;
	std::vector<MeshLoadData> meshLoadDatas;
	std::vector<ModelLoadData*> childNodes;
	XMMATRIX localMatrix;
	std::string localPath;
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
struct ComponentInitContext {
	AssetManager* assetManager;
	ResourceManager* resourceManager;
};
enum class MaterialMode
{
	Default,
	Asset,
};
struct MaterialSlot
{
	MaterialMode mode = MaterialMode::Default;

	std::string materialGUID;

	Material* localMaterial = nullptr;
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
	std::string materialName;
};
