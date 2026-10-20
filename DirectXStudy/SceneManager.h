#pragma once
#include <vector>
#include <string>
#include "CameraObject.h"
#include "LightObject.h"
#include <functional>
#include "ObjectManager.h"

class EventData;

class ModelCreater;

class SceneManager : public ObjectManager {
public:
	SceneManager(ID3D11Device* device);
	void ObjectsUpdate(float deltaTime) const override;
	void ObjectsLateUpdate(float deltaTime)const override;
	std::unordered_map<uint64_t, Object*>* GetObjects() override;
	std::unordered_map<uint64_t, Object*>* GetAllObjects() override;
	void CreateObjectSelect(std::string path);
	void SetHierarchyDropObject(Object* parentObject, Object* childObject);
	CameraObject* GetCamera() { return camera; }
	LightObject* GetLight() { return light; }
	Object* GetObjectByID(uint64_t id) override;
private:

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