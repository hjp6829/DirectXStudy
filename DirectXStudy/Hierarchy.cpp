#include "Hierarchy.h"
#include "imgui_impl_win32.h"
#include "SceneModel.h"

Hierarchy::Hierarchy(std::vector<SceneModel*>& modelContainer)
	: modelContainer(modelContainer)
{
}

void Hierarchy::UpdateUI()
{
	if (ImGui::Begin("Hierarchy"))
	{
		if (modelContainer.size() == 0)
		{
			ImGui::End();
			return;
		}
		for (int i = 0; i < modelContainer.size(); i++)
		{
			if (modelContainer[i] != NULL)
				ModelTraversal(modelContainer[i]);
		}
	}
	ImGui::End();
}

void Hierarchy::ModelTraversal(SceneModel* sceneModel)
{
	if (sceneModel->childModels.size() == 0)
	{
		ImGui::TreeNodeEx((void*)(intptr_t)sceneModel, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", sceneModel->modelName.c_str());

		if (ImGui::IsItemClicked())
		{
			OnHierarchyClick(sceneModel);
		}
		if (ImGui::BeginPopupContextItem())
		{
			OnHierarchyClick(sceneModel); 

			if (ImGui::MenuItem("Delete"))
			{
				OnHierarchyDeleteClick(sceneModel);
			}
			if (ImGui::MenuItem("Save"))
			{
				OnHierarchySaveClick(sceneModel);
			}
			if (ImGui::MenuItem("Move"))
			{
				OnHierarchyMoveClick(sceneModel);
			}

			if (ImGui::MenuItem("Rename"))
			{
				OnHierarchyRenameClick(sceneModel);
			}

			ImGui::EndPopup();
		}
		return;
	}
	bool open = ImGui::TreeNodeEx((void*)(intptr_t)sceneModel, ImGuiTreeNodeFlags_OpenOnArrow, "%s", sceneModel->modelName.c_str());
	if (ImGui::IsItemClicked())
	{
		OnHierarchyClick(sceneModel);
	}
	if (ImGui::BeginPopupContextItem())
	{
		OnHierarchyClick(sceneModel); // ��Ŭ���� ��嵵 ����

		if (ImGui::MenuItem("Delete"))
		{
			OnHierarchyDeleteClick(sceneModel);
		}

		if (ImGui::MenuItem("Save"))
		{
			OnHierarchySaveClick(sceneModel);
		}
		if (ImGui::MenuItem("Move"))
		{
			OnHierarchyMoveClick(sceneModel);
		}

		if (ImGui::MenuItem("Rename"))
		{
			OnHierarchyRenameClick(sceneModel);
		}

		ImGui::EndPopup();
	}
	if (open)
	{
		for (int i = 0; i < sceneModel->childModels.size(); i++)
		{
			ModelTraversal(sceneModel->childModels[i]);
		}
		ImGui::TreePop();
	}
}
