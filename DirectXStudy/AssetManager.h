#pragma once
#include <string>
#include <unordered_map>

struct MetaData;
enum class AssetType;

class AssetManager
{
public:
	AssetManager();
	std::unordered_map<std::string, MetaData>& GetMetaDataMapByPath() { return metaDataMapPath; }
	std::unordered_map<std::string, MetaData>& GetMetaDataMapByGuid() { return metaDataMapGuid; }
	MetaData* GetMetaDataByGUID(const std::string& guid);
	MetaData* GetMetaDataByPath(const std::string& path);
	static std::string GenerateGUID();
private:
	void CreateMetaData(const std::string& assetPath, AssetType assetType);
	AssetType GetAssetTypeFromExtension(const std::string& extension);
	void LoadMetaDataFromFile(const std::string& metaFilePath);
	std::unordered_map<std::string, MetaData> metaDataMapPath;
	std::unordered_map<std::string, MetaData> metaDataMapGuid;
};