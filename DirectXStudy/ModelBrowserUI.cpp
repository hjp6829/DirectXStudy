#include "ModelBrowserUI.h"
#include "imgui_impl_win32.h"
#include "DirectXMain.h"
#include "Datas.h"

ModelBrowserUI::ModelBrowserUI(const std::unordered_map<std::string, MetaData>& metaDataMap) : metaDataMap(metaDataMap)
{
	currentFolder = "Assets";
}

void ModelBrowserUI::UpdateUI()
{
	if (ImGui::Begin("ModelBrowserUI"))
	{
		std::vector<const MetaData*> currentFolderAssets = GetCurrentFolderAssets();

		for (const MetaData* data : currentFolderAssets)
		{
			std::string fileName = data->assetPath.filename().string();
			ImGui::Selectable(fileName.c_str());
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				switch (data->assetType)
				{
				case AssetType::FolderAssetType:
					currentFolder = data->assetPath;
					break;
				case AssetType::ModelAssetType:
					OnCreateObjectSelected(data);
				break;
				default:
					break;
				}
			}
		}
	}
	ImGui::End();
}
std::vector<const MetaData*> ModelBrowserUI::GetCurrentFolderAssets() const
{
	std::vector<const MetaData*> result;

	for (const auto& [guid, data] : metaDataMap)
	{
		if (data.assetPath.parent_path() == currentFolder)
		{
			result.push_back(&data);
		}
	}

	return result;
}