#pragma once
#include <vector>

class Object;
class EventData;

class Insfector {
public:
	Insfector();
	void UpdateUI();
	void ObjectDelete();
private:
	Object* currentObject;
	std::vector<int> meshMaterials;
	void RegisterEventHandlers();
	void SetSelectObject(EventData* eventData);
};