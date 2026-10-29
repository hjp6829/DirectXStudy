#include "RuntimeManager.h"
#include "Object.h"
#include "EventBus.h"
#include "EventDefinitions.h"
#include "Log.h"
#include "ModelCreater.h"
#include "CameraComponent.h"
#include "LightComponent.h"

RuntimeManager::RuntimeManager(ID3D11Device* device) : ObjectManager(device)
{

}
void RuntimeManager::ObjectsUpdate(float deltaTime)
{
	for (auto& [key, value] : allObjects)
	{
		value->UpdateComponent(deltaTime);
	}
	for (auto& [key, value] : objects)
	{
		value->UpdateObject();
	}
}
void RuntimeManager::ObjectsLateUpdate(float deltaTime)
{
	for (auto& [key, value] : allObjects)
	{
		value->LateUpateComponent(deltaTime);
	}
	for (auto& [key, value] : objects)
	{
		value->UpdateObject();
	}
}
std::unordered_map<uint64_t, Object*>* RuntimeManager::GetObjects()
{
	return &objects;
}
std::unordered_map<uint64_t, Object*>* RuntimeManager::GetAllObjects()
{
	return &allObjects;
}
void RuntimeManager::SetHierarchyDropObject(Object* parentObject, Object* childObject)
{
	childObject->RemoveFromParent();
	if (parentObject == nullptr)
	{
		childObject->parentObject = nullptr;
		childObject->parentobjectID = 0;
		childObject->SetRootNodeCheck(true);
		objects[childObject->objectID] = childObject;
		return;
	}

	childObject->parentObject = parentObject;
	childObject->parentobjectID = parentObject->objectID;
	XMMATRIX oldWorldMatrix = childObject->GetTransform()->GetWorldMatrix();
	XMMATRIX parentWorldMatrix = XMMatrixIdentity();
	if (parentObject != nullptr)
	{
		parentWorldMatrix = parentObject->GetTransform()->GetWorldMatrix();
	}
	childObject->GetTransform()->UpdateTransformForNewParent(oldWorldMatrix, parentWorldMatrix);
	parentObject->childObjects.push_back(childObject);

	if (childObject->IsRootObject())
	{
		objects.erase(childObject->objectID);
		childObject->SetRootNodeCheck(false);
	}
}
void RuntimeManager::StartRuntime(std::unordered_map<uint64_t, Object*>* sceneObjects)
{
	for (auto& [key, value] : *sceneObjects)
	{		
		Object* objctTemp = new Object();
		value->SetDataToCopy(objctTemp);
		allObjects[objctTemp->objectID] = objctTemp;

		std::vector<std::unique_ptr<Component>>* tempComponent = objctTemp->GetComponents();

		for (int i = 0; i < (*tempComponent).size(); i++)
		{
			if (auto* comComp = dynamic_cast<CameraComponent*>((*tempComponent)[i].get()); comComp&& comComp->isSceneCamera == false)
			{
				cameras.push_back(comComp);
			}
		}

		if (auto* light = dynamic_cast<LightComponent*>(value); light)//test
			this->light = light;
	}

	for (auto& [key, value] : allObjects)
	{
		Object* objctTemp = value;
		if(objctTemp->IsRootObject())
		{
			objects[objctTemp->objectID] = objctTemp;
			continue;
		}
		uint64_t parentID = objctTemp->parentobjectID;
		allObjects[parentID]->childObjects.push_back(objctTemp);
		objctTemp->parentObject = allObjects[parentID];
	}
}

void RuntimeManager::EndRunTime()
{
	cameras.clear();
	for (auto& [key, value] : allObjects)
	{
		delete(value);
	}
	allObjects.clear();
	objects.clear();
	light = nullptr;
}
