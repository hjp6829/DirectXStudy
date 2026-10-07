#include "SceneManager.h"
#include "Log.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "Datas.h"
#include "ModelCreater.h"
#include "UIManager.h"
#include <string>
#include "SceneObject.h"

SceneManager::SceneManager(ID3D11Device* device)
{
	modelCreater = new ModelCreater(device);
	LoadSaveSceneFile();
}

void SceneManager::SetKeyInput(int key, bool value)
{
	if(key == 17)
		isCtrl = value;
	if ((char)key == 'S' && value)
	{
		if(isCtrl)
			SaveScene();
	}
	if ((char)key == 'E' && value)
	{
		LoadSaveSceneFile();
	}
	if ((char)key == 'C' && value)
	{
		if (isCtrl)
			DuplicateObjectSave();
	}
	if ((char)key == 'V' && value)
	{
		if (isCtrl)
			DuplicateObject();
	}
}

void SceneManager::SetKeyInputHold(int key)
{
	camera->KeyboardEvent(key);
}

void SceneManager::CreateObjectSelect(std::string path)
{
	Object* object = modelCreater->LoadModelFromFile(path);
	object->SetRootNodeCheck(true);
	RegisterModelHierarchy(object);
	objects.push_back(object);
}
void SceneManager::RegisterModelHierarchy(Object* object)
{
	if (object->childObjects.size() == 0)
	{
		objectIDs.push_back(objectID);
		object->objectID = ++objectID;
		return;
	}
	objectIDs.push_back(objectID);
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
	objects.push_back(camera);
	objects.push_back(light);
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
	objects.push_back(copyObject);
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
void SceneManager::DeleteModel(Object* object)
{
	if (object->IsRootObject())
	{
		objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
	}
	object->RemoveModelData();
	DeleteChiledModels(object);
	object->childObjects.clear();
	delete object;
}
void SceneManager::TestSaveMoveParent(Object* object)
{
	testMoveChild->parentObject->RemoveChildObject(testMoveChild);
	testMoveChild->parentObject = object;
	testMoveChild->parentobjectID = object->objectID;
	testMoveChild->UpdateTransformForNewParent(object);
	object->childObjects.push_back(testMoveChild);
}
void SceneManager::DeleteChiledModels(Object* object)
{
	for (int i = 0; i < object->childObjects.size(); i++)
	{
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
	for (int i = 0; i < objects.size(); i++)
	{
		Object* object = objects[i];
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
			objects.push_back(object);
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
