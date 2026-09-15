#include "SceneManager.h"
#include "Log.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "Datas.h"
#include "ModelCreater.h"
#include "UIManager.h"

void to_json(nlohmann::json& j, const JsonSceneObjectData& data);
void from_json(const nlohmann::json& j, JsonSceneObjectData& data);

SceneManager::SceneManager(ID3D11Device* device)
{
	modelCreater = new ModelCreater(device);
	LoadSaveSceneFile();
}

void SceneManager::SetKeyInput(int key, bool value)
{
	if ((char)key == 'Q' && value)
	{
		SaveScene();
	}
	if ((char)key == 'E' && value)
	{
		LoadSaveSceneFile();
	}
}

void SceneManager::ModelSelected(std::string path)
{
	SceneObject* sceneObject = modelCreater->LoadModelFromFile(path);
	sceneObject->SetRootNodeCheck(true);
	RegisterModelHierarchy(sceneObject);
	models.push_back(sceneObject);
}
void SceneManager::RegisterModelHierarchy(SceneObject* model)
{
	if (model->childModels.size() == 0)
	{
		modelIDs.push_back(objectID);
		model->modelID = objectID++;
		return;
	}
	modelIDs.push_back(objectID);
	model->modelID = objectID++;
	for (int i = 0; i < model->childModels.size(); i++)
	{
		SceneObject* childObject = model->childModels[i];
		childObject->parentModelID = model->modelID;
		RegisterModelHierarchy(model->childModels[i]);
	}
}
void SceneManager::DeleteModel(SceneObject* model)
{
	if (model->IsRootModel())
	{
		models.erase(std::remove(models.begin(), models.end(), model), models.end());
	}
	model->RemoveModelData();
	DeleteChiledModels(model);
	model->childModels.clear();
	delete model;
}
void SceneManager::DeleteChiledModels(SceneObject* model)
{
	for (int i = 0; i < model->childModels.size(); i++)
	{
		DeleteChiledModels(model->childModels[i]);
		delete model->childModels[i];
	}
	model->childModels.clear();
}

void SceneManager::SaveScene()
{
	Log::PrintLog("Scene Save");
	std::vector<JsonSceneObjectData> jsonSceneObjectDatas;
	for (int i = 0; i < models.size(); i++)
	{
		SceneObject* model = models[i];
		SaveSceneObjectData(model, jsonSceneObjectDatas);
	}
	std::filesystem::path savePath = std::filesystem::path("SaveScene") / "Scene.json";
	std::filesystem::create_directories(savePath.parent_path());
	std::ofstream file(savePath);

	nlohmann::json jsonfile = jsonSceneObjectDatas;
	file << jsonfile.dump(4);
	file.close();
}

void SceneManager::SaveSceneObjectData(SceneObject* Model, std::vector<JsonSceneObjectData>& jsonSceneObjectDatas)
{
	JsonSceneObjectData jsonData;
	if (Model->childModels.size() == 0)
	{
		if (Model->parentModel == nullptr)
			jsonData.parentObjectID = 0;
		else
			jsonData.parentObjectID = Model->parentModelID;
		jsonData.origModelPath = Model->currentModelNode->sourceModelPath;
		jsonData.modelHeshCode = Model->currentModelNode->modelHeshCode;
		jsonData.testModelName = Model->currentModelNode->modelName;
		jsonData.objectID = Model->modelID;
		jsonData.isRootObject = Model->parentModel == nullptr ? 1 : 0;
		jsonData.SetTransformData(Model);
		jsonSceneObjectDatas.push_back(jsonData);
		return;
	}

	if (Model->parentModel == nullptr)
		jsonData.parentObjectID = 0;
	else
		jsonData.parentObjectID = Model->parentModelID;
	jsonData.origModelPath = Model->currentModelNode->sourceModelPath;
	jsonData.modelHeshCode = Model->currentModelNode->modelHeshCode;
	jsonData.testModelName = Model->currentModelNode->modelName;
	jsonData.objectID = Model->modelID;
	jsonData.isRootObject = Model->IsRootModel() == true ? 1 : 0;
	jsonData.SetTransformData(Model);
	jsonSceneObjectDatas.push_back(jsonData);
	for (int i = 0; i < Model->childModels.size(); i++)
	{
		SaveSceneObjectData(Model->childModels[i], jsonSceneObjectDatas);
	}
}

void to_json(nlohmann::json& j, const JsonSceneObjectData& data)
{
	j = nlohmann::json{
		{"modelHeshCode", data.modelHeshCode},
		{"parentObjectID", data.parentObjectID},
		{"posX", data.localPosx},
		{"posY", data.localPosy},
		{"posZ", data.localPosz},
		{ "rotx", data.localRotx },
		{"roty", data.localRoty},
		{"rotz", data.localRotz},
		{ "scalex", data.localScalex },
		{"scaley", data.localScaley},
		{"scalez", data.localScalez},
		{"ModelNamePath", data.origModelPath},
		{ "TestModelNamePath", data.testModelName },
		{ "objectID", data.objectID },
		{ "isRootObject", data.isRootObject}
	};
}

void SceneManager::LoadSaveSceneFile()
{
	using json = nlohmann::json;
	std::filesystem::path path = std::filesystem::current_path();
	std::filesystem::path saveFilePath = path / "SaveScene"/ "Scene.json";
	std::ifstream file(saveFilePath);
	if (!file.is_open())
	{
		Log::PrintLog("path errer");
	}
	json sceneJson;
	file >> sceneJson;

	std::filesystem::path modelFolderPath = "Assets/DirectXModel";

	std::unordered_map<uint64_t, SceneObject*>loadSceneObjects;
	for (const auto& item : sceneJson)
	{
		JsonSceneObjectData data = item.get<JsonSceneObjectData>();
		std::filesystem::path origModelPath = data.origModelPath;
		std::filesystem::path modelPath = path / modelFolderPath / origModelPath;
		SceneObject* SceneObject = modelCreater->CreateSceneObjectFromJsonData(modelPath, data);
		SceneObject->parentModelID = data.parentObjectID;
		SceneObject->SetPostionOffset(XMFLOAT3(data.localPosx, data.localPosy, data.localPosz));
		SceneObject->SetRotationOffset(XMFLOAT3(data.localRotx, data.localRoty, data.localRotz));
		SceneObject->SetScaleOffset(XMFLOAT3(data.localScalex, data.localScaley, data.localScalez));
		SceneObject->modelID = data.objectID;
		SceneObject->SetRootNodeCheck(data.isRootObject);
		loadSceneObjects.insert({ SceneObject->modelID, SceneObject });
		if (data.isRootObject == 1)
			models.push_back(SceneObject);
	}

	for (auto& [hashCode, model] : loadSceneObjects)
	{
		if (model->IsRootModel())
			continue;
		SceneObject* parentModel = loadSceneObjects.at(model->parentModelID);
		parentModel->InsertChildSceneObject(model);
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
