#pragma once
#include <functional>	

class Hierarchy;
class ModelBrowserUI;
class Insfector;

class UIManager {
public:
	UIManager(std::vector<SceneModel*>& sceneModels);
	void UpdateUI();
	std::function<void(std::string)> OnModelSelected;
private:
	Hierarchy* hierarchy;
	ModelBrowserUI* modelBrowserUI;
	Insfector* insfector;
};