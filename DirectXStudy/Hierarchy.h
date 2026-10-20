#pragma once
#include "Datas.h"
#include <functional>
#include <unordered_map>

class Object;
class EventData;
class WorldManager;

class Hierarchy
{
public: 
	Hierarchy(WorldManager* worldManager);
	void UpdateUI();
	std::function<void(Object*, Object*)> OnHierarchyDrop;
	std::function<void(Object*)> OnHierarchyRenameClick;
private:
	std::unordered_map<uint64_t, Object*>* currentObjects;
	WorldManager* worldManager;
	void ModelTraversal(Object* modelData);
	Object* currentSelectObject;
	void RegisterEventHandlers();
	void SetSelectObjectID(EventData* eventData);
	void DeleteCurrentObject(EventData* eventData);
};