#pragma once
#include <d3d11.h>
#include <unordered_map>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")

class TextureCreater;
class ModelCreater;
struct MetaData;
class AssimpConverter;
class AssetManager;
class ModelAsset;

using namespace Microsoft::WRL;

class ResourceManager
{
public:
	ResourceManager(ID3D11Device* device, AssetManager* assetManager);
	~ResourceManager();
	ModelAsset* LoadModelAsset(const MetaData* metaData);
private:
	TextureCreater* textureCreater;
	ModelCreater* modelCreater;
	AssimpConverter* assimp;
	AssetManager* assetManager;
	ID3D11Device* device;
private:
	std::unordered_map<std::string,ComPtr<ID3D11Texture2D>> texturesMap;
	std::unordered_map<std::string, ModelAsset*> modelAssetsMap;
};