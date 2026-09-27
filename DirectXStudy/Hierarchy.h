#pragma once
#include "Datas.h"
#include <functional>

class SceneObject;

class Hierarchy
{
public: 
	Hierarchy(std::vector<SceneObject*>* modelContainer);
	void UpdateUI();
	void SubscribeOnHierarchyMoveChildClick(std::function<void(SceneObject*)> function) {
		OnHierarchyMoveChildClick.push_back(function);
	}
	void SubscribeOnHierarchyMoveParentClick(std::function<void(SceneObject*)> function) {
		OnHierarchyMoveParentClick.push_back(function);
	}
	void RaiseOnHierarchyMoveChild(SceneObject* object);
	void RaiseOnHierarchyMoveParent(SceneObject* object);
	std::function<void(SceneObject*)> OnHierarchyClick;
	std::function<void(SceneObject*)> OnHierarchyDeleteClick;
	
	std::function<void(SceneObject*)> OnHierarchyRenameClick;
private:
	std::vector<SceneObject*>* modelContainer;
	std::vector<std::function<void(SceneObject*)>> OnHierarchyMoveChildClick;
	std::vector<std::function<void(SceneObject*)>> OnHierarchyMoveParentClick;
	void ModelTraversal(SceneObject* modelData);
};