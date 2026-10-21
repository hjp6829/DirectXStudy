#pragma once
#include <vector>
#include <unordered_map>

class EventData;
class WorldManager;

class Insfector {
public:
	Insfector(WorldManager* worldManager);
	void UpdateUI();
private:
	WorldManager* worldManager;
	uint64_t currentObjectID = -1;
	std::vector<int> meshMaterials;
	void RegisterEventHandlers();
	void SetSelectObjectID(EventData* eventData);
	void DeleteObject(EventData* eventData);
};