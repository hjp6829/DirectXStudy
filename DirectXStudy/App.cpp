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
#include "SceneManager.h"

App::App()
{
	Log* log = new Log();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	mouse = new Mouse();
	keyboard = new Keyboard();

	window = new Window(mouse, keyboard);
	dxdMain = new DirectXMain(window->GetHwnd());
	sceneManager = new SceneManager(dxdMain->GetDevice());
	dxdMain->SetSceneModels(sceneManager->GetSceneModels());
	uimanager = new UIManager(sceneManager->GetSceneModels());

	cam = new Camera(keyboard);
	light = new Light();

	dxdMain->SetCamera(cam);
	dxdMain->SetLight(light);

	uimanager->OnModelSelected = [this](std::string path) {
		sceneManager->ModelSelected(path);
		};
	uimanager->OnModelDelete = [this](SceneModel* model) {
		sceneManager->DeleteModel(model);
		};
	//uimanager->OnHierarchySaveClick = [this](SceneModel* model) {
	//	sceneManager->SaveModel(model);
	//	};
	//uimanager->OnHierarchyMoveClick = [this](SceneModel* model) {
	//	MoveModel(model);
	//	};
	//uimanager->OnHierarchyRenameClick = [this](SceneModel* model) {
	//	RenameModel(model);
	//	};
	keyboard->OnKeyPressed = [this](int key, bool value)
		{
			sceneManager->SetKeyInput(key,value);
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



//void App::SaveModel(SceneModel* model)
//{
//	testSaveModel = model;
//}
//
//void App::MoveModel(SceneModel* model)
//{
//	testSaveModel->parentModel->RemoveChildModel(testSaveModel);
//	testSaveModel->parentModel = nullptr;
//	model->childModels.push_back(testSaveModel);
//	testSaveModel = nullptr;
//
//}
//
//void App::RenameModel(SceneModel* model)
//{
//}


