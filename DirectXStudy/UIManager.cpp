#include "UIManager.h"
#include "Hierarchy.h"
#include "DirectXMain.h"
#include "ModelBrowserUI.h"
#include "Insfector.h"
#include "Object.h"
#include "Log.h"

UIManager::UIManager(std::vector<Object*>* SceneObjects)
{
	hierarchy = new Hierarchy(SceneObjects);
	modelBrowserUI = new ModelBrowserUI();
	insfector = new Insfector();

	modelBrowserUI->OnCreateObjectSelected = [this](std::string path) {
		OnCreateObjectSelected(path);
		};
	hierarchy->OnHierarchyDeleteClick = [this](Object* object) {
		insfector->ObjectDelete();
		OnModelDelete(object);
		};
	hierarchy->OnHierarchyRenameClick = [this](Object* object) {
		OnHierarchyRenameClick(object);
		};
	hierarchy->OnHierarchyClick = [this](Object* object) {
		OnHierarchyObjectClick(object);
		insfector->SetSceneObjectData(object);
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
