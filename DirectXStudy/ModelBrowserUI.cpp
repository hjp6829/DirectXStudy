#include "ModelBrowserUI.h"
#include "imgui_impl_win32.h"
#include "DirectXMain.h"

ModelBrowserUI::ModelBrowserUI()
{
	std::filesystem::path modelFolderPath = "DirectXModel";
	std::filesystem::path path = std::filesystem::current_path();
	std::filesystem::path parent2 = path.parent_path().parent_path();
	for (const auto& entry : fs::recursive_directory_iterator(parent2 / modelFolderPath))
	{
		if (!entry.is_regular_file())
			continue;

		if (entry.path().extension() == ".FBX")
		{
			files.push_back(entry.path());
		}
	}
}

void ModelBrowserUI::UpdateUI()
{
	if (ImGui::Begin("ModelBrowserUI"))
	{
		for (size_t i = 0; i < files.size(); i++)
		{
			if (ImGui::Button(files[i].filename().string().c_str()))
			{
				OnModelSelected(files[i].string());
			}
		}
	}
	ImGui::End();
}
