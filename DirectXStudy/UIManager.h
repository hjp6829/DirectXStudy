#pragma once
#include <functional>	
#include <string>
#include <vector>
#include "EventBus.h"
#include "ObjectType.h"
#include <unordered_map>

class Hierarchy;
class ModelBrowserUI;
class Insfector;
class Object;
class WorldManager;
struct MetaData;

class UIManager {
public:
	UIManager(WorldManager* worldManager, std::unordered_map<std::string, MetaData>& metaDataMap);
	void UpdateUI();
	std::function<void(const MetaData* metaData)> OnCreateObjectSelected;
	std::function<void(Object*)> OnHierarchyRenameClick;
	std::function<void(Object*, Object*)> OnHierarchyDrop;
	std::function<void(ObjectType)> OnCreateObject;
private:
	Hierarchy* hierarchy;
	ModelBrowserUI* modelBrowserUI;
	Insfector* insfector;
	ActiveSceneMode activeSceneMode;
};