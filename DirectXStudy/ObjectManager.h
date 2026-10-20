#pragma once
#include <d3d11.h>
#include <unordered_map>

class Object;

class ObjectManager
{
public:
	virtual void ObjectsUpdate(float deltaTime) const = 0;
	virtual void ObjectsLateUpdate(float deltaTime)const = 0;
	virtual std::unordered_map<uint64_t, Object*>* GetObjects(){};
	virtual std::unordered_map<uint64_t, Object*>* GetAllObjects(){};
	virtual Object* GetObjectByID(uint64_t id) {};
protected:
	std::unordered_map<uint64_t, Object*> objects;
	std::unordered_map<uint64_t, Object*> allObjects;
};