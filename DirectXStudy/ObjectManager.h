#pragma once
#include <d3d11.h>
#include <unordered_map>
#include "Log.h"
#include "ObjectType.h"

class Object;
class LightComponent;
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
	LightComponent* GetLight() {return light; }
	void SetLight(LightComponent* light){this->light = light;}
	Object* CreateObject(Object* parentObject = nullptr);
	void SetNewObject(Object* object);
	uint64_t cameraID=0;
	void ClearAllObject(){ objects.clear(); allObjects.clear(); }
	void ClearRootObject(){ objects.clear(); }
protected:
	std::unordered_map<uint64_t, Object*> objects;
	std::unordered_map<uint64_t, Object*> allObjects;
	std::vector<ICamera*> cameras;
	LightComponent* light;
	uint64_t objectID = 0;
};