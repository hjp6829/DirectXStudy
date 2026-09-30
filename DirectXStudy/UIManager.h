#pragma once
#include <functional>	
#include <string>
#include <vector>

class Hierarchy;
class ModelBrowserUI;
class Insfector;
class Object;

class UIManager {
public:
	UIManager(std::vector<Object*>* SceneObjects);
	void UpdateUI();
	std::function<void(std::string)> OnModelSelected;
	std::function<void(Object*)> OnModelDelete;
	std::function<void(Object*)> OnHierarchyMoveChildClick;
	std::function<void(Object*)> OnHierarchyMoveParentClick;
	std::function<void(Object*)> OnHierarchyRenameClick;
private:
	Hierarchy* hierarchy;
	ModelBrowserUI* modelBrowserUI;
	Insfector* insfector;
};