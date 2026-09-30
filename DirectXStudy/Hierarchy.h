#pragma once
#include "Datas.h"
#include <functional>

class Object;

class Hierarchy
{
public: 
	Hierarchy(std::vector<Object*>* modelContainer);
	void UpdateUI();
	void SubscribeOnHierarchyMoveChildClick(std::function<void(Object*)> function) {
		OnHierarchyMoveChildClick.push_back(function);
	}
	void SubscribeOnHierarchyMoveParentClick(std::function<void(Object*)> function) {
		OnHierarchyMoveParentClick.push_back(function);
	}
	void RaiseOnHierarchyMoveChild(Object* object);
	void RaiseOnHierarchyMoveParent(Object* object);
	std::function<void(Object*)> OnHierarchyClick;
	std::function<void(Object*)> OnHierarchyDeleteClick;
	
	std::function<void(Object*)> OnHierarchyRenameClick;
private:
	std::vector<Object*>* modelContainer;
	std::vector<std::function<void(Object*)>> OnHierarchyMoveChildClick;
	std::vector<std::function<void(Object*)>> OnHierarchyMoveParentClick;
	void ModelTraversal(Object* modelData);
};