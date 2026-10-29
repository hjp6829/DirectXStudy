#pragma once
#include <vector>
#include <string>
#include <functional>
#include "ObjectManager.h"
#include <functional>

class EventData;

class SceneManager : public ObjectManager {
public:
	SceneManager(ID3D11Device* device);
	void ObjectsUpdate(float deltaTime) override;
	void ObjectsLateUpdate(float deltaTime)override;
	std::unordered_map<uint64_t, Object*>* GetObjects() override;
	std::unordered_map<uint64_t, Object*>* GetAllObjects() override;
	void SetHierarchyDropObject(Object* parentObject, Object* childObject);
	std::function<void()> OnSaveScene;
private:
	bool isCtrl;
	Object* currentSelectObject;
	Object* currentDuplicateObject;
	float totalTime;
private:
	void DeleteChiledModels(Object* object);
	void CreateInitJsonFile();
	void DuplicateObjectSave();
	void DuplicateObject();
	Object* DuplicateSceneObject(Object* object);
};