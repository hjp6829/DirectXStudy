#pragma once
#include <filesystem>

namespace fs = std::filesystem;

class DirectXMain;

class ModelBrowserUI
{
public:
	ModelBrowserUI(DirectXMain* dxd);
	void UpdateUI();
private:
	std::vector<fs::path> files;
	DirectXMain* dxdMain;
private:
	void LoadModel(std::wstring path);
};