#include "SceneManager.h"
#include "Log.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "Datas.h"
#include "ModelCreater.h"
#include "UIManager.h"
#include <string>

void from_json(const nlohmann::json& j, JsonSceneObjectData& data);

SceneManager::SceneManager(ID3D11Device* device)
{
	camera = new CameraObject();
	camera->objectName = "Camera";
	light = new LightObject();
	light->objectName = "Light";
	modelCreater = new ModelCreater(device);
	objects.push_back(camera);
	objects.push_back(light);
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
}

void SceneManager::SetKeyInputHold(int key)
{
	camera->KeyboardEvent(key);
}

void SceneManager::ModelSelected(std::string path)
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
		object->objectID = objectID++;
		return;
	}
	objectIDs.push_back(objectID);
	object->objectID = objectID++;
	for (int i = 0; i < object->childObjects.size(); i++)
	{
		Object* childObject = object->childObjects[i];
		childObject->parentobjectID = object->objectID;
		RegisterModelHierarchy(object->childObjects[i]);
	}
}
Object* SceneManager::CreateSceneObject(const nlohmann::json& jsonfile,int type, std::filesystem::path path)
{
	std::filesystem::path modelFolderPath = "Assets/DirectXModel";
	std::filesystem::path origModelPath = jsonfile["origModelPath"];
	std::filesystem::path modelPath = path / modelFolderPath / origModelPath;
	switch (type)
	{
		case 0:
			Object* object = modelCreater->CreateSceneObjectFromJsonData(modelPath, jsonfile.at("modelHeshCode"));
			object->Deserialize(jsonfile);
			return object;
		case 1:
			CameraObject* camera = new CameraObject();
			camera->Deserialize(jsonfile);
			return camera;
		case 2:
			LightObject * light = new LightObject();
			light->Deserialize(jsonfile);
			return light;
	}
	return nullptr;
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
		Log::PrintLog("No SaveFile");
		return;
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

		//object->parentobjectID = data.parentObjectID;
		//object->SetPostionOffset(XMFLOAT3(data.localPosx, data.localPosy, data.localPosz));
		//object->SetRotationOffset(XMFLOAT3(data.localRotx, data.localRoty, data.localRotz));
		//object->SetScaleOffset(XMFLOAT3(data.localScalex, data.localScaley, data.localScalez));
		//object->objectID = data.objectID;
		//object->SetRootNodeCheck(data.isRootObject);
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
	loadSceneObjects.clear();
}

void from_json(const nlohmann::json& j, JsonSceneObjectData& data)
{
	j.at("modelHeshCode").get_to(data.modelHeshCode);
	j.at("parentObjectID").get_to(data.parentObjectID);

	j.at("posX").get_to(data.localPosx);
	j.at("posY").get_to(data.localPosy);
	j.at("posZ").get_to(data.localPosz);

	j.at("rotx").get_to(data.localRotx);
	j.at("roty").get_to(data.localRoty);
	j.at("rotz").get_to(data.localRotz);

	j.at("scalex").get_to(data.localScalex);
	j.at("scaley").get_to(data.localScaley);
	j.at("scalez").get_to(data.localScalez);

	j.at("ModelNamePath").get_to(data.origModelPath);
	j.at("TestModelNamePath").get_to(data.testModelName);
	j.at("objectID").get_to(data.objectID);
	j.at("isRootObject").get_to(data.isRootObject);
}
