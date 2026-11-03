#pragma once
#include <string>
#include <unordered_map>

struct MetaData;
enum class AssetType;

class AssetManager
{
public:
	AssetManager();
	std::unordered_map<std::string, MetaData>& GetMetaDataMap() { return metaDataMap; }
	MetaData* GetMetaData(const std::string& guid);
private:
	std::string GenerateGUID();
	void CreateMetaData(const std::string& assetPath, AssetType assetType);
	AssetType GetAssetTypeFromExtension(const std::string& extension);
	void LoadMetaDataFromFile(const std::string& metaFilePath);
	std::unordered_map<std::string, MetaData> metaDataMap;
};