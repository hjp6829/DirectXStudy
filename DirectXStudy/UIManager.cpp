#include "UIManager.h"
#include "Hierarchy.h"
#include "DirectXMain.h"
#include "ModelBrowserUI.h"
#include "Insfector.h"
#include "Object.h"
#include "EventDefinitions.h"
#include "imgui_impl_win32.h"
#include "WorldManager.h"

UIManager::UIManager(WorldManager* worldManager)
{
	hierarchy = new Hierarchy(worldManager);
	modelBrowserUI = new ModelBrowserUI();
	insfector = new Insfector(worldManager);

	modelBrowserUI->OnCreateObjectSelected = [this](std::string path) {
		OnCreateObjectSelected(path);
		};
	hierarchy->OnHierarchyRenameClick = [this](Object* object) {
		OnHierarchyRenameClick(object);
		};

	hierarchy->OnHierarchyDrop = [this](Object* parentObject, Object* childObject) {
		OnHierarchyDrop(parentObject, childObject);
		};

}

void UIManager::UpdateUI()
{
	hierarchy->UpdateUI();
	modelBrowserUI->UpdateUI();
	insfector->UpdateUI();
	if (ImGui::Begin("PlayMode"))
	{
		if (ImGui::Button("ModeChange"))
		{
			if (activeSceneMode == ActiveSceneMode::SceneMode)
			{

				activeSceneMode = ActiveSceneMode::RuntimeMode;
			}
			else
			{

				activeSceneMode = ActiveSceneMode::SceneMode;
			}
			ActiveSceneChangedEventData activeSceneModeClass;
			activeSceneModeClass.activeSceneMode = activeSceneMode;
			EventBus::Publish(EventType::ActiveSceneChanged, &activeSceneModeClass);
		}
		if (activeSceneMode == ActiveSceneMode::SceneMode)
			ImGui::Text("SceneMode");
		else
			ImGui::Text("PlayMode");
	}
	ImGui::End();
}
