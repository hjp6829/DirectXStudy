#include "UIManager.h"
#include "Hierarchy.h"
#include "DirectXMain.h"
#include "ModelBrowserUI.h"
#include "Insfector.h"
#include "SceneObject.h"

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
	hierarchy->OnHierarchySaveClick = [this](SceneObject* model) {
		OnHierarchySaveClick(model);
		};
	hierarchy->OnHierarchyMoveClick = [this](SceneObject* model) {
		OnHierarchyMoveClick(model);
		};
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
