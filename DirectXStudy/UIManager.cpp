#include "UIManager.h"
#include "Hierarchy.h"
#include "DirectXMain.h"
#include "ModelBrowserUI.h"
#include "Insfector.h"
#include "SceneObject.h"
#include "Log.h"

UIManager::UIManager(std::vector<SceneObject*>* SceneObjects)
{
	hierarchy = new Hierarchy(SceneObjects);
	modelBrowserUI = new ModelBrowserUI();
	insfector = new Insfector(hierarchy);

	modelBrowserUI->OnModelSelected = [this](std::string path) {
		OnModelSelected(path);
		};
	hierarchy->OnHierarchyDeleteClick = [this](SceneObject* model) {
		OnModelDelete(model);
		};
	hierarchy->SubscribeOnHierarchyMoveChildClick([this](SceneObject* model) {
		OnHierarchyMoveChildClick(model);
		});
	hierarchy->SubscribeOnHierarchyMoveParentClick([this](SceneObject* model) {
		OnHierarchyMoveParentClick(model);
		});
	hierarchy->OnHierarchyRenameClick = [this](SceneObject* model) {
		OnHierarchyRenameClick(model);
		};
}

void UIManager::UpdateUI()
{
	hierarchy->UpdateUI();
	modelBrowserUI->UpdateUI();
	insfector->UpdateUI();
}
