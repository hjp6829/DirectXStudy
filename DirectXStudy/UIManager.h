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
	UIManager(std::vector<SceneModel*>* sceneModels);
	void UpdateUI();
	std::function<void(std::string)> OnModelSelected;
	std::function<void(SceneModel*)> OnModelDelete;
	std::function<void(SceneModel*)> OnHierarchySaveClick;
	std::function<void(SceneModel*)> OnHierarchyMoveClick;
	std::function<void(SceneModel*)> OnHierarchyRenameClick;
private:
	Hierarchy* hierarchy;
	ModelBrowserUI* modelBrowserUI;
	Insfector* insfector;
};