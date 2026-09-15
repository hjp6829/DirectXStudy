#pragma once
#include <vector>

class SceneObject;
class Hierarchy;

class Insfector {
public:
	Insfector(Hierarchy* hierarchy);
	void SetSceneObjectData(SceneObject* SceneObject);
	void UpdateUI();
private:
	SceneObject* currentSceneObject;
	std::vector<int> meshMaterials;
};