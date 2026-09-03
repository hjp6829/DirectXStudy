#pragma once
#include <functional>	
#include <string>
#include <vector>

class Hierarchy;
class ModelBrowserUI;
class Insfector;
class SceneModel;

class UIManager {
public:
	UIManager(std::vector<SceneModel*>& sceneModels);
	void UpdateUI();
	std::function<void(std::string)> OnModelSelected;
	std::function<void(SceneModel*)> OnModelDelete;
private:
	Hierarchy* hierarchy;
	ModelBrowserUI* modelBrowserUI;
	Insfector* insfector;
};