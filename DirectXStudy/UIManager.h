#pragma once
#include <functional>	
#include <string>
#include <vector>

class Hierarchy;
class ModelBrowserUI;
class Insfector;
class SceneObject;

class UIManager {
public:
	UIManager(std::vector<SceneObject*>* SceneObjects);
	void UpdateUI();
	std::function<void(std::string)> OnModelSelected;
	std::function<void(SceneObject*)> OnModelDelete;
	std::function<void(SceneObject*)> OnHierarchySaveClick;
	std::function<void(SceneObject*)> OnHierarchyMoveClick;
	std::function<void(SceneObject*)> OnHierarchyRenameClick;
private:
	Hierarchy* hierarchy;
	ModelBrowserUI* modelBrowserUI;
	Insfector* insfector;
};