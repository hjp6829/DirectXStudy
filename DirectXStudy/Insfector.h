#pragma once
#include <vector>

class SceneModel;
class Hierarchy;

class Insfector {
public:
	Insfector(Hierarchy* hierarchy);
	void SetSceneModelData(SceneModel* sceneModel);
	void UpdateUI();
private:
	SceneModel* currentSceneModel;
	std::vector<int> meshMaterials;
};