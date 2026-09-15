#pragma once
#include "Datas.h"
#include <functional>

class SceneObject;

class Hierarchy
{
public: 
	Hierarchy(std::vector<SceneObject*>* modelContainer);
	void UpdateUI();
	std::function<void(SceneObject*)> OnHierarchyClick;
	std::function<void(SceneObject*)> OnHierarchyDeleteClick;
	std::function<void(SceneObject*)> OnHierarchySaveClick;
	std::function<void(SceneObject*)> OnHierarchyMoveClick;
	std::function<void(SceneObject*)> OnHierarchyRenameClick;
private:
	std::vector<SceneObject*>* modelContainer;
	void ModelTraversal(SceneObject* modelData);
};