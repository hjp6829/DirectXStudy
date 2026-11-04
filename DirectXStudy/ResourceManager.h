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

using namespace Microsoft::WRL;

class ResourceManager
{
public:
	ResourceManager(ID3D11Device* device, AssetManager* assetManager);
	~ResourceManager();
	void CreateModel(const MetaData* metaData);
private:
	TextureCreater* textureCreater;
	ModelCreater* modelCreater;
	AssimpConverter* assimp;
	AssetManager* assetManager;
private:
	std::unordered_map<std::string,ComPtr<ID3D11Texture2D>> texturesMap;
};