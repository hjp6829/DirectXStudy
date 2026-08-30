#pragma once
#include <vector>

class SceneModel;
class Hierarchy;
class AssimpConverter;

class Insfector {
public:
	Insfector(Hierarchy* hierarchy, AssimpConverter* assimp);
	void SetSceneModelData(SceneModel* sceneModel);
	void UpdateUI();
private:
	SceneModel* currentSceneModel;
	std::vector<int> meshMaterials;
	AssimpConverter* assimp;
};