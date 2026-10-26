#pragma once
#include <d3d11.h>
#include <unordered_map>
#include "ObjectType.h"
#include "ObjectManager.h"
#include "EventDefinitions.h"

class SceneManager;
class RuntimeManager;
class Object;
class EventData;
class ICamera;
class ScriptRegistry;
class ModelCreater;

class WorldManager {
public:
	WorldManager(ID3D11Device* device);
	void ObjectsUpdate(float deltaTime);
	void ObjectsLateUpdate(float deltaTime);
	std::unordered_map<uint64_t, Object*>* GetRootObjects(){return currentManager->GetObjects();}
	std::unordered_map<uint64_t, Object*>* GetAllObjects() { return currentManager->GetAllObjects(); }
	Object* GetObjectByID(uint64_t id){return currentManager->GetObjectByID(id); }
	ICamera* GetCamera(int idx);
	LightObject* GetLight(){return currentManager->GetLight(); }
	void SetCreateObject(std::string path);
	void CreateObject(ObjectType type);
	uint64_t GetSelectedObjectID(){return selectedObjectID; }
private:
	SceneManager* sceneManager;
	RuntimeManager* runtimeManager;
	ObjectManager* currentManager;
	ScriptRegistry* scriptRegistry;
	ActiveSceneMode activeSceneMode;
	uint64_t selectedObjectID;	
	ModelCreater* modelCreater;
private:
	void RegisterEventHandlers();
	void SetActiveMode(EventData* eventData);
	void SetSelectObjectID(EventData* eventData);
	void DeleteObject(EventData* eventData);
	void DeleteChiledModels(Object* object);
	void LoadSaveSceneFile();
};