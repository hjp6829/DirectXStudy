#include "Insfector.h"
#include "imgui_impl_win32.h"
#include "ModelCreater.h"
#include "SceneObject.h"
#include "Hierarchy.h"
#include "Mesh.h"


Insfector::Insfector(Hierarchy* hierarchy)
{
	hierarchy->OnHierarchyClick = [this](SceneObject* model) {
		SetSceneObjectData(model);
		};
}

void Insfector::SetSceneObjectData(SceneObject* SceneObject)
{
	currentSceneObject = SceneObject;
}

void Insfector::UpdateUI()
{
	if (currentSceneObject == NULL)
	{
		if (ImGui::Begin("Insfector"))
		{
		}
		ImGui::End();
		return;
	}
	if (ImGui::Begin("Insfector"))
	{
		XMFLOAT3 tempPos = currentSceneObject->GetModelPosition();
		XMFLOAT3 tempRot = currentSceneObject->GetModelRotation();
		XMFLOAT3 tempScale = currentSceneObject->GetModelScale();

		bool enableValue = currentSceneObject->meshEnable;
		ImGui::Checkbox("Enable",&enableValue);
		if(enableValue != currentSceneObject->meshEnable)
			currentSceneObject->ToggleMeshEnable(enableValue);
		//ImGui::Text(currentSceneObject->modelName.c_str());
		ImGui::Text("Psotion");
		if(ImGui::InputFloat3("Position", &tempPos.x))
			currentSceneObject->SetPosition(tempPos);
		ImGui::Text("Rotation");
		if(ImGui::InputFloat3("Rotation", &tempRot.x))
			currentSceneObject->SetRotaion(tempRot);
		ImGui::Text("Scale");
		if(ImGui::InputFloat3("Scale", &tempScale.x))
			currentSceneObject->SetScale(tempScale);

		//if (currentSceneObject->currentMeshs.size() != 0)
		//{
		//	ImGui::Text("Material");
		//	for (int i = 0; i < currentSceneObject->currentMeshs.size(); i++)
		//	{
		//		Mesh* meshTemp = currentSceneObject->currentMeshs[i];
		//		meshMaterials.push_back(meshTemp->GetMaterialIDX());
		//	}
		//	for (int i = 0; i < meshMaterials.size(); i++)
		//	{
		//		int temp = meshMaterials[i];
		//		ImGui::InputInt("MaterialIDX", &temp);
		//		if(meshMaterials[i] != temp)
		//			currentSceneObject->currentMeshs[i]->SetMaterialIDX(assimp->GetMaterial(temp), temp);
		//	}
		//}
		meshMaterials.clear();
	}
	ImGui::End();
}
