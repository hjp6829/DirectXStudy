#pragma once
#include "Datas.h"
#include <functional>

class SceneModel;

class Hierarchy
{
public: 
	Hierarchy(std::vector<SceneModel*>& modelContainer);
	void UpdateUI();
	std::function<void(SceneModel*)> OnHierarchyClick;
	std::function<void(SceneModel*)> OnHierarchyDeleteClick;
private:
	std::vector<SceneModel*>& modelContainer;
	void ModelTraversal(SceneModel* modelData);
};