#include "ObjectManager.h"
#include "Object.h"
ObjectManager::ObjectManager(ID3D11Device* device)
{	
	
}

Object* ObjectManager::CreateObject(Object* parentObject)
{
	Object* tempObject = new Object();
	tempObject->objectID = ++objectID;
	if(parentObject == nullptr)
	{
		tempObject->SetRootNodeCheck(true);
		objects[tempObject->objectID] = tempObject;
	}
	else
		parentObject->InsertChildSceneObject(tempObject);
	allObjects[tempObject->objectID] = tempObject;

	return tempObject;
}

void ObjectManager::SetNewObject(Object* object)
{
	if(object->IsRootObject())
		objects[object->objectID] = object;
	allObjects[object->objectID] = object;
}
