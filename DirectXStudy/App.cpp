#include "App.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "AssimpConverter.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "UIManager.h"
#include "ModelCreater.h"

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

	uimanager->OnModelSelected = [this](std::string modelName) {
		ModelSelected(modelName);
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

void App::ModelSelected(std::string modelName)
{
	// 모델 선택 시 처리할 로직을 여기에 작성합니다.
}
