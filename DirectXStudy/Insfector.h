#pragma once
#include <vector>
#include <unordered_map>

class Object;
class EventData;
class ScriptRegistry;

class Insfector {
public:
	Insfector(std::unordered_map<uint64_t, Object*>* modelContainer, std::unordered_map<uint64_t, Object*>* runtimeObjects);
	void UpdateUI();
private:
	std::unordered_map<uint64_t, Object*>* currentObjects;
	std::unordered_map<uint64_t, Object*>* sceneObjects;
	std::unordered_map<uint64_t, Object*>* runtimeObjects;
	Object* currentObject;
	std::vector<int> meshMaterials;
	void RegisterEventHandlers();
	void SetSelectObject(EventData* eventData);
	void DeleteObject(EventData* eventData);
	void ActiveModeChange(EventData* eventData);
	ScriptRegistry* scriptRegistry;
};