#pragma once
#include "Datas.h"
#include <functional>
#include <unordered_map>

class Object;
class EventData;

class Hierarchy
{
public: 
	Hierarchy(std::unordered_map<uint64_t, Object*>* modelContainer, std::unordered_map<uint64_t, Object*>* runtimeObjects);
	void UpdateUI();
	std::function<void(Object*, Object*)> OnHierarchyDrop;
	std::function<void(Object*)> OnHierarchyRenameClick;
private:
	std::unordered_map<uint64_t, Object*>* currentObjects;
	std::unordered_map<uint64_t, Object*>* sceneObjects;
	std::unordered_map<uint64_t, Object*>* runtimeObjects;
	void ModelTraversal(Object* modelData);
	Object* currentSelectObject;
	void RegisterEventHandlers();
	void SetSelectObject(EventData* eventData);
	void DeleteCurrentObject(EventData* eventData);
	void SetRuntimeObjects(EventData* eventData);
};