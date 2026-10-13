#include "App.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "AssimpConverter.h"
#include "Mouse.h"
#include "Input.h"
#include "UIManager.h"
#include "ModelCreater.h"
#include "Object.h"
#include "ModelNode.h"
#include "Datas.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "SceneManager.h"
#include "EventBus.h"

App::App()
{
	RegisterEventHandlers();
	Log* log = new Log();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	mouse = new Mouse();
	input = new Input();

	window = new Window(mouse, input);
	dxdMain = new DirectXMain(window->GetHwnd());
	sceneManager = new SceneManager(dxdMain->GetDevice());
	dxdMain->SetSceneObjects(sceneManager->GetSceneObjects());
	uimanager = new UIManager(sceneManager->GetSceneObjects());

	dxdMain->SetCamera(sceneManager->GetCamera());
	dxdMain->SetLight(sceneManager->GetLight());

	uimanager->OnCreateObjectSelected = [this](std::string path) {//이벤트 버스
		sceneManager->CreateObjectSelect(path);
		};
	uimanager->OnHierarchyDrop = [this](Object* parentObject, Object* childObject) {//이벤트 버스
		sceneManager->SetHierarchyDropObject(parentObject, childObject);
		};
	//uimanager->OnHierarchyRenameClick = [this](SceneObject* model) {
	//	RenameModel(model);
	//	};

	mouse->OnMouseLeftDown = [this](bool value)
		{
			dxdMain->MouseLeftClick(value);
			
		};
	mouse->OnMouseRightDown = [this](bool value)
	{
		sceneManager->SetMouseRightDown(value);
		
	};
	mouse->OnMouseWheelDown = [this](bool value)
		{
			sceneManager->SetMouseWheelDown(value);
		};
	mouse->OnMouseWheelDelta = [this](float value)
		{
			sceneManager->SetMouseWheelDelta(value);
			//Log::GetInstance()->LogMessage("Mouse Wheel Delta: " + std::to_string(delta));
		};
	mouse->OnMouseDeltaPos = [this](int x,int y)
		{
			sceneManager->SetMouseDeltaPos(x,y);
			//Log::GetInstance()->LogMessage("Mouse Wheel Delta: " + std::to_string(delta));
		};
	mouse->OnMousePos = [this](int x, int y)
		{
			dxdMain->MousePickingPos(x,y);
		};
}

void App::Run()
{
	auto previousTime = std::chrono::steady_clock::now();
	MSG msg;

	dxdMain->Start();

	while (GetMessage(&msg, NULL, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = currentTime - previousTime;
		float deltaTime = elapsed.count();
		sceneManager->Update(deltaTime);
		input->Update();
		dxdMain->Render();
		mouse->SetWheelDelta(0);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		uimanager->UpdateUI();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		dxdMain->EndDraw();

		for (int i = 0; i < commandQueue.size(); i++)
		{
			commandQueue.front()();
			commandQueue.pop();
		}

		previousTime = currentTime;
	}
}



//void App::SaveModel(SceneObject* model)
//{
//	testSaveModel = model;
//}
//
//void App::MoveModel(SceneObject* model)
//{
//	testSaveModel->parentModel->RemoveChildObject(testSaveModel);
//	testSaveModel->parentModel = nullptr;
//	model->childObjects.push_back(testSaveModel);
//	testSaveModel = nullptr;
//
//}
//
void App::RegisterEventHandlers()
{
	EventBus::Subscribe(EventType::RequestDeleteObject, [this](EventData* data) {DeleteObject(data); });
}
void App::DeleteObject(EventData* eventData)
{
	SelectObjectIDData* eventDataCast = static_cast<SelectObjectIDData*>(eventData);

	if (eventDataCast != nullptr)
	{
		uint64_t objectID = eventDataCast->objectID;
		commandQueue.push([this, objectID]() {
			SelectObjectIDData selectObjectData;
			selectObjectData.objectID = objectID;
			EventBus::Publish(EventType::ObjectWillBeDeleted, &selectObjectData);
			});
	}

}


