#include "Hierarchy.h"
#include "imgui_impl_win32.h"
#include "SceneObject.h"

Hierarchy::Hierarchy(std::vector<SceneObject*>* modelContainer)
	: modelContainer(modelContainer)
{
}

void Hierarchy::UpdateUI()
{
	if (ImGui::Begin("Hierarchy"))
	{
		if (modelContainer->size() == 0)
		{
			ImGui::End();
			return;
		}
		for (int i = 0; i < modelContainer->size(); i++)
		{
			if ((*modelContainer)[i] != NULL)
				ModelTraversal((*modelContainer)[i]);
		}
	}
	ImGui::End();
}

void Hierarchy::ModelTraversal(SceneObject* SceneObject)
{
	if (SceneObject->childModels.size() == 0)
	{
		ImGui::TreeNodeEx((void*)(intptr_t)SceneObject, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", SceneObject->modelName.c_str());

		if (ImGui::IsItemClicked())
		{
			OnHierarchyClick(SceneObject);
		}
		if (ImGui::BeginPopupContextItem())
		{
			OnHierarchyClick(SceneObject); 

			if (ImGui::MenuItem("Delete"))
			{
				OnHierarchyDeleteClick(SceneObject);
			}
			if (ImGui::MenuItem("Save"))
			{
				OnHierarchySaveClick(SceneObject);
			}
			if (ImGui::MenuItem("Move"))
			{
				OnHierarchyMoveClick(SceneObject);
			}

			if (ImGui::MenuItem("Rename"))
			{
				OnHierarchyRenameClick(SceneObject);
			}

			ImGui::EndPopup();
		}
		return;
	}
	bool open = ImGui::TreeNodeEx((void*)(intptr_t)SceneObject, ImGuiTreeNodeFlags_OpenOnArrow, "%s", SceneObject->modelName.c_str());
	if (ImGui::IsItemClicked())
	{
		OnHierarchyClick(SceneObject);
	}
	if (ImGui::BeginPopupContextItem())
	{
		OnHierarchyClick(SceneObject); // ��Ŭ���� ��嵵 ����

		if (ImGui::MenuItem("Delete"))
		{
			OnHierarchyDeleteClick(SceneObject);
		}

		if (ImGui::MenuItem("Save"))
		{
			OnHierarchySaveClick(SceneObject);
		}
		if (ImGui::MenuItem("Move"))
		{
			OnHierarchyMoveClick(SceneObject);
		}

		if (ImGui::MenuItem("Rename"))
		{
			OnHierarchyRenameClick(SceneObject);
		}

		ImGui::EndPopup();
	}
	if (open)
	{
		for (int i = 0; i < SceneObject->childModels.size(); i++)
		{
			ModelTraversal(SceneObject->childModels[i]);
		}
		ImGui::TreePop();
	}
}
