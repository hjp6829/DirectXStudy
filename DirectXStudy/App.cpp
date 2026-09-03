#include "App.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "AssimpConverter.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "UIManager.h"
#include "ModelCreater.h"
#include "SceneModel.h";

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