#pragma once
#include <vector>
#include <d3d11.h>
#include <string>
#include "CameraObject.h"
#include "LightObject.h"
#include <unordered_map>
#include <functional>

class EventData;
class Object;
class ModelCreater;
struct JsonSceneObjectData;

class SceneManager {
public:
	SceneManager(ID3D11Device* device);
	void Update(float deltaTime);
	std::unordered_map<uint64_t, Object*>* GetSceneObjects() { return &objects; }
	void CreateObjectSelect(std::string path);
	void SetMouseRightDown(bool value); //{ camera->SetMouseRightValue(value); }
	void SetMouseWheelDown(bool value); //{ camera->SetMouseWheelDown(value); }
	void SetMouseWheelDelta(float delta); //{ camera->SetMouseWheelDelta(delta); }
	void SetMouseDeltaPos(int x, int y); //{ camera->SetMouseDeltaPos(x, y); }
	void SetHierarchyDropObject(Object* parentObject, Object* childObject);
	CameraObject* GetCamera() { return camera; }
	LightObject* GetLight() { return light; }
private:
	std::unordered_map<uint64_t, Object*> objects;
	std::unordered_map<uint64_t, Object*> allObjects;
	ModelCreater* modelCreater;
	uint64_t objectID = 0;
	bool isCtrl;
	CameraObject* camera;
	LightObject* light;
	bool isNoSaveFile;
	Object* currentSelectObject;
	Object* currentDuplicateObject;
	float totalTime;
private:
	void SaveScene();
	void LoadSaveSceneFile();
	void SaveSceneObjectData(Object* object, nlohmann::json& jsonfile);
	void DeleteChiledModels(Object* object);
	void RegisterModelHierarchy(Object* object);
	Object* CreateSceneObject(const nlohmann::json& jsonfile, int type, std::filesystem::path path);
	void CreateInitJsonFile();
	void DuplicateObjectSave();
	void DuplicateObject();
	Object* DuplicateSceneObject(Object* object);
	void RegisterEventHandlers();
	void SetSelectObjectID(EventData* eventData);
	void SetSelectObject(EventData* eventData);
	void DeleteObject(EventData* eventData);
};