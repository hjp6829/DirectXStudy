#include "UIManager.h"
#include "Hierarchy.h"
#include "DirectXMain.h"
#include "ModelBrowserUI.h"
#include "Insfector.h"
#include "SceneModel.h"

UIManager::UIManager(std::vector<SceneModel*>* sceneModels)
{
	hierarchy = new Hierarchy(sceneModels);
	modelBrowserUI = new ModelBrowserUI();
	insfector = new Insfector(hierarchy);

	modelBrowserUI->OnModelSelected = [this](std::string path) {
		OnModelSelected(path);
		};
	hierarchy->OnHierarchyDeleteClick = [this](SceneModel* model) {
		OnModelDelete(model);
		};
	hierarchy->OnHierarchySaveClick = [this](SceneModel* model) {
		OnHierarchySaveClick(model);
		};
	hierarchy->OnHierarchyMoveClick = [this](SceneModel* model) {
		OnHierarchyMoveClick(model);
		};
	hierarchy->OnHierarchyRenameClick = [this](SceneModel* model) {
		OnHierarchyRenameClick(model);
		};
}

void UIManager::UpdateUI()
{
	hierarchy->UpdateUI();
	modelBrowserUI->UpdateUI();
	insfector->UpdateUI();
}
