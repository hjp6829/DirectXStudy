#include "App.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "AssimpConverter.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "UIManager.h"
#include "ModelCreater.h"
#include "SceneModel.h";
#include "ModelNode.h"
#include "Datas.h"
#include <fstream>
#include <nlohmann/json.hpp>

void to_json(nlohmann::json& j, const JsonSceneModelData& data);

App::App()
{
	Log* log = new Log();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	mouse = new Mouse();
	keyboard = new Keyboard();

	window = new Window(mouse, keyboard);
	dxdMain = new DirectXMain(window->GetHwnd(), models);
	modelCreater = new ModelCreater(dxdMain->GetDevice());
	uimanager = new UIManager(models);

	cam = new Camera(keyboard);
	light = new Light();

	dxdMain->SetCamera(cam);
	dxdMain->SetLight(light);

	uimanager->OnModelSelected = [this](std::string path) {
		ModelSelected(path);
		};
	uimanager->OnModelDelete = [this](SceneModel* model) {
		DeleteModel(model);
		};
	uimanager->OnHierarchySaveClick = [this](SceneModel* model) {
		SaveModel(model);
		};
	uimanager->OnHierarchyMoveClick = [this](SceneModel* model) {
		MoveModel(model);
		};
	uimanager->OnHierarchyRenameClick = [this](SceneModel* model) {
		RenameModel(model);
		};
	keyboard->OnKeyPressed = [this](int key, bool value)
		{
			this->SaveScene(key, value);
			this->LoadSaveSceneFile(key, value);
		};
}

void App::Run()
{
	auto previousTime = std::chrono::steady_clock::now();
	MSG msg;

	dxdMain->Start();
	dxdMain->SetMouse(window->GetMouse());

	while (GetMessage(&msg, NULL, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = currentTime - previousTime;
		float deltaTime = elapsed.count();
		dxdMain->Update(deltaTime);
		dxdMain->Render();

		cam->CameraUpdate(mouse);
		keyboard->Update();
		mouse->SetWheelDelta(0);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		cam->ViewCameraWindow();
		light->ViewLightWindow();
		uimanager->UpdateUI();
		//static bool showDemoWindow = true;
		//ImGui::ShowDemoWindow(&showDemoWindow);
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		dxdMain->EndDraw();
		previousTime = currentTime;
	}
}

void App::ModelSelected(std::string path)
{
	SceneModel* sceneModel = modelCreater->LoadModelFromFile(path);
	models.push_back(sceneModel);
}

void App::DeleteModel(SceneModel* model)
{
	if (model->IsRootModel())
	{
		models.erase(std::remove(models.begin(), models.end(), model), models.end());
	}
	model->RemoveModelData();
	DeleteChiledModels(model);
	model->childNodes.clear();
	delete model;
}

void App::DeleteChiledModels(SceneModel* model)
{
	for (int i = 0; i < model->childNodes.size(); i++)
	{
		DeleteChiledModels(model->childNodes[i]);
		delete model->childNodes[i];
	}
	model->childNodes.clear();
}

void App::SaveModel(SceneModel* model)
{
	testSaveModel = model;
}

void App::MoveModel(SceneModel* model)
{
	testSaveModel->parentModel->RemoveChildModel(testSaveModel);
	testSaveModel->parentModel = nullptr;
	model->childNodes.push_back(testSaveModel);
	testSaveModel = nullptr;

}

void App::RenameModel(SceneModel* model)
{
}

void App::SaveScene(int key,bool value)
{
	if ((char)key == 'Q'&& value)
	{
		Log::PrintLog("Scene Save");
		std::vector<JsonSceneModelData> jsonSceneModelDatas;
		for (int i = 0; i < models.size(); i++)
		{
			SceneModel* model = models[i];
			SaveSceneModelData(model, jsonSceneModelDatas);
		}
		std::filesystem::path savePath = "C:/Users/박현진/Desktop/Project/DirectX/DirectXStudy/SaveScene/Scene.json";
		std::ofstream file(savePath);

		nlohmann::json jsonfile = jsonSceneModelDatas;
		file << jsonfile.dump(4);
		file.close();
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
		{"ModelNamePath", data.origModelName}
	};
}

void App::SaveSceneModelData(SceneModel* Model, std::vector<JsonSceneModelData>& jsonSceneModelDatas)
{
	JsonSceneModelData jsonData;
	if (Model->childNodes.size() == 0)
	{
		if (Model->parentModel == nullptr)
			jsonData.parentModelHeshCode = 0;
		else
			jsonData.parentModelHeshCode = Model->parentModel->currentModelNode->modelHeshCode;
		jsonData.origModelName = Model->currentModelNode->sourceModelPath;
		jsonData.modelHeshCode = Model->currentModelNode->modelHeshCode;
		jsonData.SetTransformData(Model);
		jsonSceneModelDatas.push_back(jsonData);
		return;
	}

	if(Model->parentModel == nullptr)
		jsonData.parentModelHeshCode = 0;
	else
		jsonData.parentModelHeshCode = Model->parentModel->currentModelNode->modelHeshCode;
	jsonData.origModelName = Model->currentModelNode->sourceModelPath;
	jsonData.modelHeshCode = Model->currentModelNode->modelHeshCode;
	jsonData.SetTransformData(Model);
	jsonSceneModelDatas.push_back(jsonData);
	for (int i = 0; i < Model->childNodes.size(); i++)
	{
		SaveSceneModelData(Model->childNodes[i], jsonSceneModelDatas);
	}
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

	j.at("ModelNamePath").get_to(data.origModelName);
}

void App::LoadSaveSceneFile(int key, bool value)
{
	if ((char)key == 'E' && value)
	{
		using json = nlohmann::json;
		std::filesystem::path path = std::filesystem::current_path();
		std::filesystem::path parent1 = path.parent_path();
		std::filesystem::path parent2 = path.parent_path().parent_path();
		std::filesystem::path saveFolderPath = "SaveScene/Scene.json";

		std::filesystem::path savePath = parent1/ saveFolderPath;
		std::ifstream file(savePath);
		if (!file.is_open())
		{
			Log::PrintLog("path errer");
		}
		json sceneJson;
		file >> sceneJson;

		std::filesystem::path modelFolderPath = "DirectXModel";
		

		for (const auto& item : sceneJson)
		{
			JsonSceneModelData data = item.get<JsonSceneModelData>();
			std::filesystem::path modelName = data.origModelName;
			std::filesystem::path modelPath = parent2 / modelFolderPath / modelName;
			SceneModel* sceneModel = modelCreater->CreateSceneModelFromJsonData(modelPath, data);
			Log::PrintLog(sceneModel->modelName);
			models.push_back(sceneModel);
		}
	}
}
