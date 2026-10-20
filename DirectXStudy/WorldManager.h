#pragma once
#include <d3d11.h>
#include <unordered_map>

class SceneManager;
class RuntimeManager;
class Object;
class EventData;
class ObjectManager;

class WorldManager {
public:
	WorldManager(ID3D11Device* device);
	void ObjectsUpdate(float deltaTime);
	void ObjectsLateUpdate(float deltaTime);
	std::unordered_map<uint64_t, Object*>* GetRootObjects(){return currentRootObjects;}
	std::unordered_map<uint64_t, Object*>* GetAllObjects() { return currentAllObjects; }
	Object* GetObjectByID(uint64_t id){return currentManager->GetObjectByID(id); }
private:
	SceneManager* sceneManager;
	RuntimeManager* runtimeManager;
	ObjectManager* currentManager;

	std::unordered_map<uint64_t, Object*>* currentRootObjects;
	std::unordered_map<uint64_t, Object*>* currentAllObjects;
	std::unordered_map<uint64_t, Object*> objects;
	std::unordered_map<uint64_t, Object*> allObjects;
private:

	void RegisterEventHandlers();
	void SetActiveMode(EventData* eventData);
};