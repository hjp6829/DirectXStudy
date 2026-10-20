#pragma once
#include <functional>	
#include <string>
#include <vector>
#include "EventBus.h"

class Hierarchy;
class ModelBrowserUI;
class Insfector;
class Object;

class UIManager {
public:
	UIManager(std::unordered_map<uint64_t, Object*>* SceneObjects, std::unordered_map<uint64_t, Object*>* runtimeObjects);
	void UpdateUI();
	std::function<void(std::string)> OnCreateObjectSelected;
	std::function<void(Object*)> OnHierarchyRenameClick;
	std::function<void(Object*, Object*)> OnHierarchyDrop;
private:
	Hierarchy* hierarchy;
	ModelBrowserUI* modelBrowserUI;
	Insfector* insfector;
	ActiveSceneMode activeSceneMode;
};