#pragma once
#include <vector>

class SceneObject;
class Hierarchy;

class Insfector {
public:
	Insfector(Hierarchy* hierarchy);
	void SetSceneObjectData(SceneObject* SceneObject);
	void SettestMoveChild(SceneObject* SceneObject){ testMoveChild  = SceneObject;}
	void SettestMoveParent(SceneObject* SceneObject){ testMoveParent = SceneObject; }
	void UpdateUI();
private:
	SceneObject* currentSceneObject;
	SceneObject* testMoveChild;
	SceneObject* testMoveParent;
	std::vector<int> meshMaterials;
};