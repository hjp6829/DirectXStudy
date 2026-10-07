#pragma once
#include <filesystem>
#include <functional>

namespace fs = std::filesystem;

class DirectXMain;

class ModelBrowserUI
{
public:
	ModelBrowserUI();
	void UpdateUI();
	std::function<void(std::string)> OnCreateObjectSelected;
private:
	std::vector<fs::path> files;
};