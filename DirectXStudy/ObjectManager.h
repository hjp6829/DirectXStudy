#pragma once
#include <d3d11.h>
#include <unordered_map>
#include "CameraObject.h"
#include "LightObject.h"
#include "Log.h"
#include "ObjectType.h"

class Object;
class ModelCreater;
class ICamera;

class ObjectManager
{
public:
	ObjectManager(ID3D11Device* device);
	virtual void ObjectsUpdate(float deltaTime) = 0;
	virtual void ObjectsLateUpdate(float deltaTime) = 0;
	virtual std::unordered_map<uint64_t, Object*>* GetObjects() =0;
	virtual std::unordered_map<uint64_t, Object*>* GetAllObjects()=0;
	Object* GetObjectByID(uint64_t id) {
		auto it = allObjects.find(id);
		return it != allObjects.end() ? it->second : nullptr;
	}
	void SetCameraVector(ICamera* camera){ cameras.push_back(camera); }
	ICamera* GetCamera(int idx){return cameras[idx];}
	ICamera* GetSceneCamera(){return sceneCamera; }
	LightObject* GetLight() {return light; }
	void SetCreateObject(std::string path);
	void RegisterModelHierarchy(Object* object);
	Object* CreateObject(ObjectType type);
protected:
	std::unordered_map<uint64_t, Object*> objects;
	std::unordered_map<uint64_t, Object*> allObjects;
	std::vector<ICamera*> cameras;
	ICamera* sceneCamera;
	LightObject* light;
	ModelCreater* modelCreater;
	uint64_t objectID = 0;
};