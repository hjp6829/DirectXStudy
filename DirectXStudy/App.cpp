#include "App.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "AssimpConverter.h"
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
#include "RuntimeManager.h"
#include "Log.h"
#include "WorldManager.h"

App::App()
{
	RegisterEventHandlers();
	Log* log = new Log();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	input = new Input();

	window = new Window(input);
	dxdMain = new DirectXMain(window->GetHwnd());

	worldManager = new WorldManager(dxdMain->GetDevice());
	dxdMain->SetObjects(sceneManager->GetSceneObjects(), runtimeManager->GetSceneObjects());
	uimanager = new UIManager(sceneManager->GetSceneObjects(), runtimeManager->GetSceneObjects());

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
}

void App::Run()
{
	auto previousTime = std::chrono::steady_clock::now();
	MSG msg = {};

	dxdMain->Start();

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = currentTime - previousTime;
		float deltaTime = elapsed.count();

		worldManager->ObjectsUpdate(deltaTime);
		worldManager->ObjectsLateUpdate(deltaTime);
		dxdMain->Render();
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		uimanager->UpdateUI();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		dxdMain->EndDraw();
		input->Update();
		while (!commandQueue.empty())
		{
			auto command = std::move(commandQueue.front());
			commandQueue.pop();

			command();
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
	EventBus::Subscribe(EventType::ActiveSceneChanged, [this](EventData* data) {ActiveSceneModeChange(data); });
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

void App::ActiveSceneModeChange(EventData* eventData)
{
	ActiveSceneChangedEventData* eventDataCast = static_cast<ActiveSceneChangedEventData*>(eventData);

	//if (eventDataCast != nullptr)
	//{
	//	activeSceneMode = eventDataCast->activeSceneMode;
	//	if(activeSceneMode == ActiveSceneMode::RuntimeMode)
	//		runtimeManager->StartRuntime(sceneManager->GetSceneAllObjects());
	//}
}
