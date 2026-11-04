#include "ResourceManager.h"
#include "TextureCreater.h"
#include "ModelCreater.h"	
#include "AssimpConverter.h"
#include "Datas.h"
#include "AssetManager.h"

ResourceManager::ResourceManager(ID3D11Device* device, AssetManager* assetManager)
{
	textureCreater = new TextureCreater(device);
	modelCreater = new ModelCreater(device);
	assimp = new AssimpConverter(device);
	this->assetManager = assetManager;
}

void ResourceManager::CreateModel(const MetaData* metaData)
{
	assimp->ReadAssetFile(metaData->assetPath);
	std::vector<TextureRequest> textureRequests = assimp->ReadTextureData(metaData->assetPath);
	
	for(const auto& textureRequest : textureRequests)
	{
		MetaData* metaData = assetManager->GetMetaDataByPath(textureRequest.path.string());
		auto it = texturesMap.find(metaData->GUID);
		ID3D11Texture2D* textureTemp = nullptr;
		if (it == texturesMap.end())
		{
			ComPtr<ID3D11Texture2D> textureCom = textureCreater->CreateTextureFromFile(textureRequest.path.wstring());
			textureTemp = textureCom.Get();
			texturesMap[metaData->GUID] = textureCom;
		}
		textureTemp = it->second.Get();
	}

	modelCreater->LoadModelFromFile(metaData, assimp->CreateModelLoadData(metaData->assetPath.string()));



}
