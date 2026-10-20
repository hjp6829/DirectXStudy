#pragma once
#include <functional>	
#include <string>
#include <vector>
#include "EventBus.h"

class Hierarchy;
class ModelBrowserUI;
class Insfector;
class Object;
class WorldManager;

class UIManager {
public:
	UIManager(WorldManager* worldManager);
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