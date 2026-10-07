#pragma once
#include "Datas.h"
#include <functional>

class Object;

class Hierarchy
{
public: 
	Hierarchy(std::vector<Object*>* modelContainer);
	void UpdateUI();
	std::function<void(Object*)> OnHierarchyClick;
	std::function<void(Object*)> OnHierarchyDeleteClick;
	std::function<void(Object*, Object*)> OnHierarchyDrop;
	std::function<void(Object*)> OnHierarchyRenameClick;
private:
	std::vector<Object*>* modelContainer;
	void ModelTraversal(Object* modelData);
};