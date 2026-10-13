#include "UIManager.h"
#include "Hierarchy.h"
#include "DirectXMain.h"
#include "ModelBrowserUI.h"
#include "Insfector.h"
#include "Object.h"
#include "Log.h"

UIManager::UIManager(std::unordered_map<uint64_t, Object*>* SceneObjects)
{
	hierarchy = new Hierarchy(SceneObjects);
	modelBrowserUI = new ModelBrowserUI();
	insfector = new Insfector();

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
}
