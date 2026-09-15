#pragma once
#include "Datas.h"
#include <functional>

class SceneModel;

class Hierarchy
{
public: 
	Hierarchy(std::vector<SceneModel*>* modelContainer);
	void UpdateUI();
	std::function<void(SceneModel*)> OnHierarchyClick;
	std::function<void(SceneModel*)> OnHierarchyDeleteClick;
	std::function<void(SceneModel*)> OnHierarchySaveClick;
	std::function<void(SceneModel*)> OnHierarchyMoveClick;
	std::function<void(SceneModel*)> OnHierarchyRenameClick;
private:
	std::vector<SceneModel*>* modelContainer;
	void ModelTraversal(SceneModel* modelData);
};