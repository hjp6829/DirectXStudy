#pragma once
#include "Datas.h"
#include <functional>

class ModelContainer;
class SceneModel;

class Hierarchy
{
public: 
	Hierarchy(std::vector<ModelContainer*>& modelContainer);
	void UpdateUI();
	std::function<void(SceneModel*)> OnHierarchyClick;
private:
	std::vector<ModelContainer*>& modelContainer;
	void ModelTraversal(SceneModel* modelData);
};