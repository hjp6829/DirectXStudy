#include "ObjectManager.h"
#include "ModelCreater.h"
ObjectManager::ObjectManager(ID3D11Device* device)
{	
	modelCreater = new ModelCreater(device);
}

void ObjectManager::SetCreateObject(std::string path)
{
	Object* object = modelCreater->LoadModelFromFile(path);
	object->SetRootNodeCheck(true);
	RegisterModelHierarchy(object);
	objects[object->objectID] = object;
}

void ObjectManager::RegisterModelHierarchy(Object* object)
{
	if (object->childObjects.size() == 0)
	{
		object->objectID = ++objectID;
		allObjects[object->objectID] = object;
		return;
	}
	object->objectID = ++objectID;
	allObjects[object->objectID] = object;
	for (int i = 0; i < object->childObjects.size(); i++)
	{
		Object* childObject = object->childObjects[i];
		childObject->parentobjectID = object->objectID;
		RegisterModelHierarchy(object->childObjects[i]);
	}
}