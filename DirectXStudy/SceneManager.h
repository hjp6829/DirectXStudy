#pragma once
#include <vector>
#include <string>
#include "LightObject.h"
#include <functional>
#include "ObjectManager.h"

class EventData;

class SceneManager : public ObjectManager {
public:
	SceneManager(ID3D11Device* device);
	void ObjectsUpdate(float deltaTime) override;
	void ObjectsLateUpdate(float deltaTime)override;
	std::unordered_map<uint64_t, Object*>* GetObjects() override;
	std::unordered_map<uint64_t, Object*>* GetAllObjects() override;
	void SetHierarchyDropObject(Object* parentObject, Object* childObject);
	LightObject* GetLight() { return light; }
private:
	bool isCtrl;
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

};