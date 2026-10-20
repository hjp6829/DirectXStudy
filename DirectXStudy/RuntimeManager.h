#pragma once
#include "ObjectManager.h"

class RuntimeManager : public ObjectManager {
public:
	RuntimeManager(ID3D11Device* device);
	void ObjectsUpdate(float deltaTime) override;
	void ObjectsLateUpdate(float deltaTime) override;
	std::unordered_map<uint64_t, Object*>* GetObjects() override;
	std::unordered_map<uint64_t, Object*>* GetAllObjects() override;
	void SetHierarchyDropObject(Object* parentObject, Object* childObject);
	void StartRuntime(std::unordered_map<uint64_t, Object*>* sceneObjects);
private:
	std::unordered_map<uint64_t, Object*> objects;
	std::unordered_map<uint64_t, Object*> allObjects;
};