#include "Insfector.h"
#include "imgui_impl_win32.h"
#include "ModelCreater.h"
#include "SceneModel.h"
#include "Hierarchy.h"
#include "Mesh.h"


Insfector::Insfector(Hierarchy* hierarchy)
{
	hierarchy->OnHierarchyClick = [this](SceneModel* model) {
		SetSceneModelData(model);
		};
}

void Insfector::SetSceneModelData(SceneModel* sceneModel)
{
	currentSceneModel = sceneModel;
}

void Insfector::UpdateUI()
{
	if (currentSceneModel == NULL)
	{
		if (ImGui::Begin("Insfector"))
		{
		}
		ImGui::End();
		return;
	}
	if (ImGui::Begin("Insfector"))
	{
		XMFLOAT3 tempPos = currentSceneModel->GetModelPosition();
		XMFLOAT3 tempRot = currentSceneModel->modelRot;
		XMFLOAT3 tempScale = currentSceneModel->modelScale;

		bool enableValue = currentSceneModel->meshEnable;
		ImGui::Checkbox("Enable",&enableValue);
		if(enableValue != currentSceneModel->meshEnable)
			currentSceneModel->ToggleMeshEnable(enableValue);
		//ImGui::Text(currentSceneModel->modelName.c_str());
		ImGui::Text("Psotion");
		if(ImGui::InputFloat3("Position", &tempPos.x))
			currentSceneModel->SetPosition(tempPos);
		ImGui::Text("Rotation");
		if(ImGui::InputFloat3("Rotation", &tempRot.x))
			currentSceneModel->SetRotaion(tempRot);
		ImGui::Text("Scale");
		if(ImGui::InputFloat3("Scale", &tempScale.x))
			currentSceneModel->SetScale(tempScale);

		//if (currentSceneModel->currentMeshs.size() != 0)
		//{
		//	ImGui::Text("Material");
		//	for (int i = 0; i < currentSceneModel->currentMeshs.size(); i++)
		//	{
		//		Mesh* meshTemp = currentSceneModel->currentMeshs[i];
		//		meshMaterials.push_back(meshTemp->GetMaterialIDX());
		//	}
		//	for (int i = 0; i < meshMaterials.size(); i++)
		//	{
		//		int temp = meshMaterials[i];
		//		ImGui::InputInt("MaterialIDX", &temp);
		//		if(meshMaterials[i] != temp)
		//			currentSceneModel->currentMeshs[i]->SetMaterialIDX(assimp->GetMaterial(temp), temp);
		//	}
		//}
		meshMaterials.clear();
	}
	ImGui::End();
}
