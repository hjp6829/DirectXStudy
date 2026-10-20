#pragma once
#include <d3d11.h>
#include <unordered_map>
#include "CameraObject.h"
#include "LightObject.h"
#include "Log.h"

class Object;
class ModelCreater;

class ObjectManager
{
public:
	ObjectManager(ID3D11Device* device);
	virtual void ObjectsUpdate(float deltaTime) = 0;
	virtual void ObjectsLateUpdate(float deltaTime) = 0;
	virtual std::unordered_map<uint64_t, Object*>* GetObjects() =0;
	virtual std::unordered_map<uint64_t, Object*>* GetAllObjects()=0;
	Object* GetObjectByID(uint64_t id) { return allObjects[id]; }
	CameraObject* GetCamera(int idx){return cameras[idx];}
	LightObject* GetLight() {return light; }
	void SetCreateObject(std::string path);
	void RegisterModelHierarchy(Object* object);
protected:
	std::unordered_map<uint64_t, Object*> objects;
	std::unordered_map<uint64_t, Object*> allObjects;
	std::vector<CameraObject*> cameras;
	LightObject* light;
	ModelCreater* modelCreater;
	uint64_t objectID = 0;
};