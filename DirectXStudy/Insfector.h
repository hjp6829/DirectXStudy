#pragma once
#include <vector>

class Object;
class Hierarchy;

class Insfector {
public:
	Insfector();
	void SetSceneObjectData(Object* object);
	void UpdateUI();
	void ObjectDelete();
private:
	Object* currentObject;
	std::vector<int> meshMaterials;
};