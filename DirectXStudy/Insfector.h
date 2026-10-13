#pragma once
#include <vector>

class Object;
class EventData;
class ScriptRegistry;

class Insfector {
public:
	Insfector();
	void UpdateUI();
private:
	Object* currentObject;
	std::vector<int> meshMaterials;
	void RegisterEventHandlers();
	void SetSelectObject(EventData* eventData);
	void DeleteObject(EventData* eventData);
	ScriptRegistry* scriptRegistry;
};