#include "SceneManager.h"
#include "Log.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "Datas.h"
#include "ModelCreater.h"
#include "UIManager.h"
#include <string>
#include "SceneObject.h"
#include "EventBus.h"
#include "EventDefinitions.h"
#include "imgui_impl_dx11.h"
#include "Input.h"

SceneManager::SceneManager(ID3D11Device* device)
{
	RegisterEventHandlers();
	modelCreater = new ModelCreater(device);
	LoadSaveSceneFile();
}
void SceneManager::Update(float deltaTime)
{
	for (auto& [key, value] : allObjects)
	{
		value->UpdateComponent(deltaTime);
	}
	for (auto& [key, value] : objects)
	{
		value->UpdateObject();
	}

	if(Input::GetKey(VK_CONTROL))
		isCtrl = true;

	if (Input::GetKeyDown('E'))
		LoadSaveSceneFile();

	if (isCtrl)
	{
		if (Input::GetKeyDown('S'))
			SaveScene();
		if (Input::GetKeyDown('C'))
			DuplicateObjectSave();
		if (Input::GetKeyDown('V'))
			DuplicateObject();
	}
	isCtrl = false;
}

void SceneManager::CreateObjectSelect(std::string path)
{
	Object* object = modelCreater->LoadModelFromFile(path);
	object->SetRootNodeCheck(true);
	RegisterModelHierarchy(object);
	objects[object->objectID] = object;
}
void SceneManager::SetMouseRightDown(bool value)
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
		camera->SetMouseRightValue(value);
}
void SceneManager::SetMouseWheelDown(bool value)
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
		camera->SetMouseWheelDown(value);
}
void SceneManager::SetMouseWheelDelta(float delta)
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
		camera->SetMouseWheelDelta(delta);
}
void SceneManager::SetMouseDeltaPos(int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
		camera->SetMouseDeltaPos(x, y);
}
void SceneManager::RegisterModelHierarchy(Object* object)
{
	if (object->childObjects.size() == 0)
	{
		allObjects[object->objectID] = object;
		object->objectID = ++objectID;
		return;
	}
	allObjects[object->objectID] = object;
	object->objectID = ++objectID;
	for (int i = 0; i < object->childObjects.size(); i++)
	{
		Object* childObject = object->childObjects[i];
		childObject->parentobjectID = object->objectID;
		RegisterModelHierarchy(object->childObjects[i]);
	}
}
Object* SceneManager::CreateSceneObject(const nlohmann::json& jsonfile,int type, std::filesystem::path path)
{
	Object* object = nullptr;
	switch (type)
	{
		case 0:
			{
			std::filesystem::path modelFolderPath = "Assets/DirectXModel";
			std::string origModelPath;
			jsonfile.at("ModelNamePath").get_to(origModelPath);
			std::filesystem::path modelPath = path / modelFolderPath / origModelPath;
			object = modelCreater->CreateSceneObjectFromJsonData(modelPath, jsonfile.at("modelHeshCode"));
			object->Deserialize(jsonfile);
			break;
			}
		case 1:
			{
			camera = new CameraObject();
			camera->Deserialize(jsonfile);
			object = camera;
			break;
			}
		case 2:
			{
			light = new LightObject();
			light->Deserialize(jsonfile);
			object = light;
			break;
			}
	}
	objectID = object->objectID;

	return object;
}
void SceneManager::CreateInitJsonFile()
{
	isNoSaveFile = true;
	CameraObject* camera = new CameraObject();
	camera->objectID = ++objectID;
	LightObject* light = new LightObject();
	light->objectID = ++objectID;
	objects[camera->objectID] = camera;
	objects[light->objectID] = light;
	SaveScene();
	objects.clear();
}
void SceneManager::DuplicateObjectSave()
{
	currentDuplicateObject = currentSelectObject;
}
void SceneManager::DuplicateObject()
{
	Object* copyObject = DuplicateSceneObject(currentDuplicateObject);
	objects[copyObject->objectID] = copyObject;
}
Object* SceneManager::DuplicateSceneObject(Object* object)
{
	if(object->childObjects.size() == 0)
	{
		SceneObject* copyObject =new SceneObject();
		copyObject->objectID = ++objectID;
		modelCreater->SetModelToObject(copyObject, object->currentModelNode);
		object->SetDataToCopy(copyObject);
		return copyObject;
	}
	SceneObject* copyObject = new SceneObject();
	copyObject->objectID = ++objectID;
	modelCreater->SetModelToObject(copyObject, object->currentModelNode);
	object->SetDataToCopy(copyObject);
	for (int i = 0; i < object->childObjects.size(); i++)
	{
		Object* copyChildObject = DuplicateSceneObject(object->childObjects[i]);
		copyObject->childObjects.push_back(copyChildObject);
		copyChildObject->parentObject = copyObject;
	}
	return copyObject;	
}
void SceneManager::SetSelectObjectID(EventData* eventData)
{
	SelectObjectIDData* eventDataCast = static_cast<SelectObjectIDData*>(eventData);

	if (eventDataCast != nullptr)
	{
		currentSelectObject = allObjects[eventDataCast->objectID];
		SelectObjectData selectObjectData;
		selectObjectData.selectObject = currentSelectObject;
		EventBus::Publish(EventType::SelectObject, &selectObjectData);
	}
}
void SceneManager::SetSelectObject(EventData* eventData)
{
	SelectObjectData* eventDataCast = static_cast<SelectObjectData*>(eventData);

	if (eventDataCast != nullptr)
	{
		currentSelectObject = eventDataCast->selectObject;
	}
}
void SceneManager::DeleteObject(EventData* eventData)
{
	SelectObjectIDData* eventDataCast = static_cast<SelectObjectIDData*>(eventData);

	if (eventDataCast != nullptr)
	{
		Object* tempObject = allObjects[eventDataCast->objectID];
		if (tempObject->IsRootObject())
		{
			objects.erase(tempObject->objectID);
		}
		allObjects.erase(tempObject->objectID);
		tempObject->RemoveModelData();
		DeleteChiledModels(tempObject);
		tempObject->childObjects.clear();
		delete tempObject;
	}
}
void SceneManager::RegisterEventHandlers()
{
	EventBus::Subscribe(EventType::SelectObject, [this](EventData* data) {SetSelectObject(data); });
	EventBus::Subscribe(EventType::SelectObjectID, [this](EventData* data) {SetSelectObjectID(data); });
	EventBus::Subscribe(EventType::ObjectWillBeDeleted, [this](EventData* data) {DeleteObject(data); });
}
void SceneManager::SetHierarchyDropObject(Object* parentObject, Object* childObject)
{
	childObject->RemoveFromParent();
	if (parentObject == nullptr)
	{
		childObject->parentObject=nullptr;
		childObject->parentobjectID = 0;
		childObject->SetRootNodeCheck(true);
		objects[childObject->objectID] = childObject;
		return;
	}

	childObject->parentObject = parentObject;
	childObject->parentobjectID = parentObject->objectID;
	childObject->UpdateTransformForNewParent(parentObject);
	parentObject->childObjects.push_back(childObject);

	if (childObject->IsRootObject())
	{
		objects.erase(childObject->objectID);
		childObject->SetRootNodeCheck(false);
	}
}

void SceneManager::DeleteChiledModels(Object* object)
{
	for (int i = 0; i < object->childObjects.size(); i++)
	{
		allObjects.erase(object->childObjects[i]->objectID);
		DeleteChiledModels(object->childObjects[i]);
		delete object->childObjects[i];
	}
	object->childObjects.clear();
}

void SceneManager::SaveScene()
{
	Log::PrintLog("Scene Save");
	std::filesystem::path savePath = std::filesystem::path("SaveScene") / "Scene.json";
	std::filesystem::create_directories(savePath.parent_path());
	std::ofstream file(savePath);

	nlohmann::json jsonfile = nlohmann::json::array();
	for (auto& [key, value] : objects)
	{
		Object* object = value;
		SaveSceneObjectData(object, jsonfile);
	}
	file << jsonfile.dump(4);
	file.close();
}

void SceneManager::SaveSceneObjectData(Object* object, nlohmann::json& jsonfile)
{
	nlohmann::json objectJson;
	if (object->childObjects.size() == 0)
	{
		object->Serialize(objectJson);
		jsonfile.push_back(objectJson);
		return;
	}
	object->Serialize(objectJson);
	jsonfile.push_back(objectJson);
	for (int i = 0; i < object->childObjects.size(); i++)
	{
		SaveSceneObjectData(object->childObjects[i], jsonfile);
	}
}

void SceneManager::LoadSaveSceneFile()
{
	using json = nlohmann::json;
	std::filesystem::path path = std::filesystem::current_path();
	std::filesystem::path saveFilePath = path / "SaveScene"/ "Scene.json";
	if (!std::filesystem::exists(saveFilePath))
	{
		Log::PrintLog("No SaveFile Create Init SaveFile");
		CreateInitJsonFile();
	}
	std::ifstream file(saveFilePath);
	if (!file.is_open())
	{
		Log::PrintLog("path errer");
		return;
	}
	if (file.peek() == std::ifstream::traits_type::eof())
	{
		Log::PrintLog("file is empty");
		return;
	}
	json sceneJson;
	file >> sceneJson;

	std::unordered_map<uint64_t, Object*>loadSceneObjects;
	for (const auto& item : sceneJson)
	{
		int objectType;
		item.at("ObjectType").get_to(objectType);		
		Object* object = CreateSceneObject(item, objectType, path);

		loadSceneObjects.insert({ object->objectID, object });
		if (item.at("isRootObject") == 1)
			objects[object->objectID] = object;
		allObjects[object->objectID] = object;
	}

	for (auto& [hashCode, objectTemp] : loadSceneObjects)
	{
		if (objectTemp->IsRootObject())
			continue;
		Object* parentObject = loadSceneObjects.at(objectTemp->parentobjectID);
		parentObject->InsertChildSceneObject(objectTemp);
	}
	isNoSaveFile = false;
	loadSceneObjects.clear();
}
