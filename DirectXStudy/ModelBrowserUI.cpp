#include "ModelBrowserUI.h"
#include "imgui_impl_win32.h"
#include "DirectXMain.h"

ModelBrowserUI::ModelBrowserUI()
{
	std::filesystem::path assetsFolder = "Assets";
	std::filesystem::path path = std::filesystem::current_path();
	currentFolder = path / assetsFolder;
}

void ModelBrowserUI::UpdateUI()
{
	if (ImGui::Begin("ModelBrowserUI"))
	{
		for (const auto& entry : fs::directory_iterator(currentFolder))
		{
			const fs::path& entryPath = entry.path();
			std::string fileName = entryPath.filename().string();
			if (entry.is_directory())
			{
				if (ImGui::Selectable(("[Folder] " + fileName).c_str()))
				{
					currentFolder = entryPath;
					break;
				}
			}
			else if (entry.is_regular_file())
			{
				if (ImGui::Selectable(fileName.c_str()))
				{
					OnCreateObjectSelected(entryPath.string());
				}
			}
		}
	}
	ImGui::End();
}
