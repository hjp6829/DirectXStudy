#pragma once
#include <d3d11.h>
#include <unordered_map>

class Object;

class RuntimeManager {
public:
	RuntimeManager(ID3D11Device* device);
	void ObjectsUpdate(float deltaTime);
	void ObjectsLateUpdate(float deltaTime);
	std::unordered_map<uint64_t, Object*>* GetSceneObjects() { return &objects; }
	void SetHierarchyDropObject(Object* parentObject, Object* childObject);
	void StartRuntime(std::unordered_map<uint64_t, Object*>* sceneObjects);
private:
	std::unordered_map<uint64_t, Object*> objects;
	std::unordered_map<uint64_t, Object*> allObjects;
};