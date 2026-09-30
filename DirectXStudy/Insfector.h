#pragma once
#include <vector>

class Object;
class Hierarchy;

class Insfector {
public:
	Insfector(Hierarchy* hierarchy);
	void SetSceneObjectData(Object* object);
	void SettestMoveChild(Object* object){ testMoveChild  = object;}
	void SettestMoveParent(Object* object){ testMoveParent = object; }
	void UpdateUI();
private:
	Object* currentObject;
	Object* testMoveChild;
	Object* testMoveParent;
	std::vector<int> meshMaterials;
};