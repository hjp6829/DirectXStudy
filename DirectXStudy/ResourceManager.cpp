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

		TextureAsset textureAsset;
		device->CreateShaderResourceView(textureTemp,nullptr, textureAsset.srv.GetAddressOf());

		modelAssetTemp->SetMaterialTexture(textureRequest.materialIndex, &textureAsset, textureRequest.type);
	}

	return modelAssetTemp;
}
