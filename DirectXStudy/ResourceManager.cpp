#include "ResourceManager.h"
#include "TextureCreater.h"
#include "ModelCreater.h"	
#include "AssimpConverter.h"
#include "Datas.h"
#include "AssetManager.h"
#include "ModelAsset.h"

ResourceManager::ResourceManager(ID3D11Device* device, AssetManager* assetManager)
{
	textureCreater = new TextureCreater(device);
	modelCreater = new ModelCreater(device);
	assimp = new AssimpConverter(device);
	this->assetManager = assetManager;
	this->device = device;
}

ModelAsset* ResourceManager::LoadModelAsset(const MetaData* metaData)
{
	if (modelAssetsMap.find(metaData->GUID) != modelAssetsMap.end())
	{
		return modelAssetsMap[metaData->GUID];
	}
	assimp->ReadAssetFile(metaData->assetPath);
	ModelAsset* modelAssetTemp = modelCreater->LoadModelFromFile(metaData, assimp->CreateModelLoadData(metaData->assetPath.string()));
	modelAssetsMap[metaData->GUID] = modelAssetTemp;

	std::vector<TextureRequest> textureRequests = assimp->ReadTextureData(metaData->assetPath);
	
	for(const auto& textureRequest : textureRequests)
	{
		std::string materialName = textureRequest.materialName;
		MetaData* metaData = assetManager->GetMetaDataByPath(textureRequest.path.string());
		auto it = texturesAssetMap.find(metaData->GUID);
		if (it == texturesAssetMap.end())
		{
			TextureAsset textureAsset;
			ComPtr<ID3D11Texture2D> textureCom = textureCreater->CreateTextureFromFile(textureRequest.path.wstring());
			device->CreateShaderResourceView(textureCom.Get(), nullptr, textureAsset.srv.GetAddressOf());
			auto result = texturesAssetMap.emplace(metaData->GUID,std::move(textureAsset));
			it = result.first;
		}

		modelAssetTemp->SetMaterialTexture(textureRequest.materialIndex, &it->second, textureRequest.type, materialName);
	}

	return modelAssetTemp;
}
