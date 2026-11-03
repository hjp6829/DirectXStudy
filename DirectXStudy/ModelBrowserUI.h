#pragma once
#include <filesystem>
#include <functional>

namespace fs = std::filesystem;

class DirectXMain;
struct MetaData;

class ModelBrowserUI
{
public:
	ModelBrowserUI(const std::unordered_map<std::string, MetaData>& metaDataMap);
	void UpdateUI();
	std::function<void(const MetaData*)> OnCreateObjectSelected;
private:
	fs::path currentFolder;
	const std::unordered_map<std::string, MetaData>& metaDataMap;
	std::vector<const MetaData*> GetCurrentFolderAssets() const;
};