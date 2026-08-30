#include "UIManager.h"
#include "Hierarchy.h"
#include "DirectXMain.h"
#include "ModelBrowserUI.h"
#include "Insfector.h"

UIManager::UIManager(DirectXMain* dxd)
{
	dxdmain = dxd;
	hierarchy = new Hierarchy(dxdmain->GetSceneModels());
	modelBrowserUI = new ModelBrowserUI(dxd);
	insfector = new Insfector(hierarchy, dxdmain->GetAssimp());
}

void UIManager::UpdateUI()
{
	hierarchy->UpdateUI();
	modelBrowserUI->UpdateUI();
	insfector->UpdateUI();
}
