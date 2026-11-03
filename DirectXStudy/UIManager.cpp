#include "UIManager.h"
#include "Hierarchy.h"
#include "DirectXMain.h"
#include "ModelBrowserUI.h"
#include "Insfector.h"
#include "Object.h"
#include "EventDefinitions.h"
#include "imgui_impl_win32.h"
#include "WorldManager.h"
#include "ObjectType.h"
#include "AssetManager.h"

UIManager::UIManager(WorldManager* worldManager, std::unordered_map<std::string, MetaData>& metaDataMap)
{
	hierarchy = new Hierarchy(worldManager);
	modelBrowserUI = new ModelBrowserUI(metaDataMap);
	insfector = new Insfector(worldManager);

	modelBrowserUI->OnCreateObjectSelected = [this](const MetaData* metaData) {
		OnCreateObjectSelected(metaData);
		};
	hierarchy->OnHierarchyRenameClick = [this](Object* object) {
		OnHierarchyRenameClick(object);
		};

	hierarchy->OnHierarchyDrop = [this](Object* parentObject, Object* childObject) {
		OnHierarchyDrop(parentObject, childObject);
		};
	hierarchy->OnHierarchyCreateObjectClick = [this](ObjectType type) {
		OnCreateObject(type);
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
