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
	insfector = new Insfector(hierarchy);

	modelBrowserUI->OnModelSelected = [this](std::string path) {
		OnModelSelected(path);
		};
	hierarchy->OnHierarchyDeleteClick = [this](Object* object) {
		insfector->ObjectDelete();
		OnModelDelete(object);
		};
	hierarchy->SubscribeOnHierarchyMoveChildClick([this](Object* object) {
		OnHierarchyMoveChildClick(object);
		});
	hierarchy->SubscribeOnHierarchyMoveParentClick([this](Object* object) {
		OnHierarchyMoveParentClick(object);
		});
	hierarchy->OnHierarchyRenameClick = [this](Object* object) {
		OnHierarchyRenameClick(object);
		};
}

void UIManager::UpdateUI()
{
	hierarchy->UpdateUI();
	modelBrowserUI->UpdateUI();
	insfector->UpdateUI();
}
