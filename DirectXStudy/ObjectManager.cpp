#include "ObjectManager.h"
#include "Object.h"
ObjectManager::ObjectManager(ID3D11Device* device)
{	
	
}

Object* ObjectManager::CreateObject(Object* parentObject)
{
	Object* tempObject = new Object();
	if(parentObject != nullptr)
		parentObject->InsertChildSceneObject(tempObject);

	tempObject->SetRootNodeCheck(true);
	tempObject->objectID = ++objectID;
	objects[tempObject->objectID] = tempObject;
	return tempObject;
}

void ObjectManager::SetNewObject(Object* object)
{
	if(object->IsRootObject())
		objects[object->objectID] = object;
	allObjects[object->objectID] = object;
}
