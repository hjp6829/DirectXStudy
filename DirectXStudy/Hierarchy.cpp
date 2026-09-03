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
	if (sceneModel->childNodes.size() == 0)
	{
		ImGui::TreeNodeEx((void*)(intptr_t)sceneModel, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", sceneModel->modelName.c_str());

		if (ImGui::IsItemClicked())
		{
			OnHierarchyClick(sceneModel);
		}
		if (ImGui::BeginPopupContextItem())
		{
			OnHierarchyClick(sceneModel); // 우클릭한 노드도 선택

			if (ImGui::MenuItem("Delete"))
			{
				OnHierarchyDeleteClick(sceneModel);
			}

			if (ImGui::MenuItem("Rename"))
			{
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
		OnHierarchyClick(sceneModel); // 우클릭한 노드도 선택

		if (ImGui::MenuItem("Delete"))
		{
			OnHierarchyDeleteClick(sceneModel);
		}

		if (ImGui::MenuItem("Rename"))
		{
		}

		ImGui::EndPopup();
	}
	if (open)
	{
		for (int i = 0; i < sceneModel->childNodes.size(); i++)
		{
			ModelTraversal(sceneModel->childNodes[i]);
		}
		ImGui::TreePop();
	}
}
