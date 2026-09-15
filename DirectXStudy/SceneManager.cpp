#include "SceneManager.h"
#include "Log.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "Datas.h"
#include "ModelCreater.h"
#include "UIManager.h"

void to_json(nlohmann::json& j, const JsonSceneModelData& data);
void from_json(const nlohmann::json& j, JsonSceneModelData& data);

SceneManager::SceneManager(ID3D11Device* device)
{
	modelCreater = new ModelCreater(device);
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
	SceneModel* sceneModel = modelCreater->LoadModelFromFile(path);
	models.push_back(sceneModel);
}
void SceneManager::DeleteModel(SceneModel* model)
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
void SceneManager::DeleteChiledModels(SceneModel* model)
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
	std::vector<JsonSceneModelData> jsonSceneModelDatas;
	for (int i = 0; i < models.size(); i++)
	{
		SceneModel* model = models[i];
		SaveSceneModelData(model, jsonSceneModelDatas);
	}
	std::filesystem::path savePath = std::filesystem::path("SaveScene") / "Scene.json";
	std::filesystem::create_directories(savePath.parent_path());
	std::ofstream file(savePath);

	nlohmann::json jsonfile = jsonSceneModelDatas;
	file << jsonfile.dump(4);
	file.close();
}

void SceneManager::SaveSceneModelData(SceneModel* Model, std::vector<JsonSceneModelData>& jsonSceneModelDatas)
{
	JsonSceneModelData jsonData;
	if (Model->childModels.size() == 0)
	{
		if (Model->parentModel == nullptr)
			jsonData.parentModelHeshCode = 0;
		else
			jsonData.parentModelHeshCode = Model->parentModel->currentModelNode->modelHeshCode;
		jsonData.origModelPath = Model->currentModelNode->sourceModelPath;
		jsonData.modelHeshCode = Model->currentModelNode->modelHeshCode;
		jsonData.testModelName = Model->currentModelNode->modelName;
		jsonData.SetTransformData(Model);
		jsonSceneModelDatas.push_back(jsonData);
		return;
	}

	if (Model->parentModel == nullptr)
		jsonData.parentModelHeshCode = 0;
	else
		jsonData.parentModelHeshCode = Model->parentModel->currentModelNode->modelHeshCode;
	jsonData.origModelPath = Model->currentModelNode->sourceModelPath;
	jsonData.modelHeshCode = Model->currentModelNode->modelHeshCode;
	jsonData.testModelName = Model->currentModelNode->modelName;
	jsonData.SetTransformData(Model);
	jsonSceneModelDatas.push_back(jsonData);
	for (int i = 0; i < Model->childModels.size(); i++)
	{
		SaveSceneModelData(Model->childModels[i], jsonSceneModelDatas);
	}
}

void to_json(nlohmann::json& j, const JsonSceneModelData& data)
{
	j = nlohmann::json{
		{"modelHeshCode", data.modelHeshCode},
		{"parentModelHeshCode", data.parentModelHeshCode},
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
		{ "TestModelNamePath", data.testModelName }
	};
}

void SceneManager::LoadSaveSceneFile()
{
	using json = nlohmann::json;
	std::filesystem::path path = std::filesystem::current_path();
	std::filesystem::path parent1 = path.parent_path();
	std::filesystem::path parent2 = path.parent_path().parent_path();
	std::filesystem::path saveFolderPath = "C:/Users/Admin/Desktop/DirectXStrudy/Directx/DirectXStudy/SaveScene/Scene.json";

	//std::filesystem::path savePath = parent1/ saveFolderPath;
	std::ifstream file(saveFolderPath);
	if (!file.is_open())
	{
		Log::PrintLog("path errer");
	}
	json sceneJson;
	file >> sceneJson;

	std::filesystem::path modelFolderPath = "DirectXModel";

	std::unordered_map<uint64_t, SceneModel*>loadSceneModels;
	for (const auto& item : sceneJson)
	{
		JsonSceneModelData data = item.get<JsonSceneModelData>();
		std::filesystem::path origModelPath = data.origModelPath;
		std::filesystem::path modelPath = parent2 / modelFolderPath / origModelPath;
		SceneModel* sceneModel = modelCreater->CreateSceneModelFromJsonData(modelPath, data);
		sceneModel->parentModelHeshCode = data.parentModelHeshCode;
		sceneModel->SetPostionOffset(XMFLOAT3(data.localPosx, data.localPosy, data.localPosz));
		sceneModel->SetRotationOffset(XMFLOAT3(data.localRotx, data.localRoty, data.localRotz));
		sceneModel->SetScaleOffset(XMFLOAT3(data.localScalex, data.localScaley, data.localScalez));
		loadSceneModels.insert({ sceneModel->currentModelNode->modelHeshCode, sceneModel });
		if (sceneModel->parentModelHeshCode == 0)
			models.push_back(sceneModel);
	}

	for (auto& [hashCode, model] : loadSceneModels)
	{
		if (model->parentModelHeshCode == 0)
		{
			Log::PrintLog("parentModelHeshCode == 0 : " + model->modelName);
			continue;
		}
		SceneModel* parentModel = loadSceneModels.at(model->parentModelHeshCode);
		parentModel->InsertChildSceneModel(model);
	}
	loadSceneModels.clear();
}

void from_json(const nlohmann::json& j, JsonSceneModelData& data)
{
	j.at("modelHeshCode").get_to(data.modelHeshCode);
	j.at("parentModelHeshCode").get_to(data.parentModelHeshCode);

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
}
