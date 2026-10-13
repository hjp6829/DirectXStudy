#pragma once
#include "Datas.h"
#include <functional>

class Object;
class EventData;

class Hierarchy
{
public: 
	Hierarchy(std::unordered_map<uint64_t, Object*>* modelContainer);
	void UpdateUI();
	std::function<void(Object*, Object*)> OnHierarchyDrop;
	std::function<void(Object*)> OnHierarchyRenameClick;
private:
	std::unordered_map<uint64_t, Object*>* modelContainer;
	void ModelTraversal(Object* modelData);
	Object* currentSelectObject;
	void RegisterEventHandlers();
	void SetSelectObject(EventData* eventData);
	void DeleteCurrentObject(EventData* eventData);
};