#include "UIManager.h"
#include "Hierarchy.h"
#include "DirectXMain.h"
#include "ModelBrowserUI.h"
#include "Insfector.h"

UIManager::UIManager(std::vector<SceneModel*>& sceneModels)
{
	hierarchy = new Hierarchy(sceneModels);
	modelBrowserUI = new ModelBrowserUI();
	insfector = new Insfector(hierarchy);

	modelBrowserUI->OnModelSelected = [this](std::string modelName) {
		OnModelSelected(modelName);
		};
}

void UIManager::UpdateUI()
{
	hierarchy->UpdateUI();
	modelBrowserUI->UpdateUI();
	insfector->UpdateUI();
}
