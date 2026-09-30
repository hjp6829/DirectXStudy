#include "Insfector.h"
#include "imgui_impl_win32.h"
#include "ModelCreater.h"
#include "Object.h"
#include "Hierarchy.h"
#include "Mesh.h"
#include "Log.h"

Insfector::Insfector(Hierarchy* hierarchy)
{
	hierarchy->OnHierarchyClick = [this](Object* object) {
		SetSceneObjectData(object);
		};
	hierarchy->SubscribeOnHierarchyMoveChildClick([this](Object* object) {
		{
			Log::PrintLog(object->objectName);
			SettestMoveChild(object);
		}});
	hierarchy->SubscribeOnHierarchyMoveParentClick([this](Object* object) {
		{
			Log::PrintLog(object->objectName);
			SettestMoveParent(object);
		}});
}

void Insfector::SetSceneObjectData(Object* object)
{
	currentObject = object;
}

void Insfector::UpdateUI()
{
	if (currentObject == NULL)
	{
		if (ImGui::Begin("Insfector"))
		{
			ImGui::Text("Test");
			ImGui::Text("testMoveChild");
			ImGui::Text(testMoveChild == nullptr ? "" : (testMoveChild->objectName).c_str());
			ImGui::Text("testMoveParent");
			ImGui::Text(testMoveParent == nullptr ? "" : (testMoveParent->objectName).c_str());
		}
		ImGui::End();
		return;
	}
	if (ImGui::Begin("Insfector"))
	{
		XMFLOAT3 tempPos = currentObject->GetObjectPosition();
		XMFLOAT3 tempRot = currentObject->GetObjectRotation();
		XMFLOAT3 tempScale = currentObject->GetObjectScale();

		bool enableValue = currentObject->meshEnable;
		ImGui::Checkbox("Enable",&enableValue);
		if(enableValue != currentObject->meshEnable)
			currentObject->ToggleMeshEnable(enableValue);
		//ImGui::Text(currentSceneObject->modelName.c_str());
		ImGui::Text("Psotion");
		if(ImGui::InputFloat3("Position", &tempPos.x))
			currentObject->SetPosition(tempPos);
		ImGui::Text("Rotation");
		if(ImGui::InputFloat3("Rotation", &tempRot.x))
			currentObject->SetRotaion(tempRot);
		ImGui::Text("Scale");
		if(ImGui::InputFloat3("Scale", &tempScale.x))
			currentObject->SetScale(tempScale);

		ImGui::Text("Test");
		ImGui::Text("testMoveChild");
		ImGui::Text("%s", testMoveChild == nullptr ? "" : (testMoveChild->objectName).c_str());
		ImGui::Text("testMoveParent");
		ImGui::Text("%s", testMoveParent == nullptr ? "" : (testMoveParent->objectName).c_str());
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
