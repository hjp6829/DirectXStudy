#include "Hierarchy.h"
#include "imgui_impl_win32.h"
#include "Object.h"

Hierarchy::Hierarchy(std::vector<Object*>* modelContainer)
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

void Hierarchy::RaiseOnHierarchyMoveChild(Object* object)
{
	for (int i = 0;i < OnHierarchyMoveChildClick.size(); i++)
	{
		OnHierarchyMoveChildClick[i](object);
	}
}

void Hierarchy::RaiseOnHierarchyMoveParent(Object* object)
{
	for (int i = 0;i < OnHierarchyMoveParentClick.size(); i++)
	{
		OnHierarchyMoveParentClick[i](object);
	}
}

void Hierarchy::ModelTraversal(Object* object)
{
	if (object->childObjects.size() == 0)
	{
		ImGui::TreeNodeEx((void*)(intptr_t)object, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", object->objectName.c_str());

		if (ImGui::IsItemClicked())
		{
			OnHierarchyClick(object);
		}
		if (ImGui::BeginPopupContextItem())
		{
			OnHierarchyClick(object);

			if (ImGui::MenuItem("Delete"))
			{
				OnHierarchyDeleteClick(object);
			}
			if (ImGui::MenuItem("SetMoveChild"))
			{
				RaiseOnHierarchyMoveChild(object);
			}
			if (ImGui::MenuItem("SetMoveParent"))
			{
				RaiseOnHierarchyMoveParent(object);
			}

			if (ImGui::MenuItem("Rename"))
			{
				OnHierarchyRenameClick(object);
			}

			ImGui::EndPopup();
		}
		return;
	}
	bool open = ImGui::TreeNodeEx((void*)(intptr_t)object, ImGuiTreeNodeFlags_OpenOnArrow, "%s", object->objectName.c_str());
	if (ImGui::IsItemClicked())
	{
		OnHierarchyClick(object);
	}
	if (ImGui::BeginPopupContextItem())
	{
		OnHierarchyClick(object); // ��Ŭ���� ��嵵 ����

		if (ImGui::MenuItem("Delete"))
		{
			OnHierarchyDeleteClick(object);
		}

		if (ImGui::MenuItem("SetMoveChild"))
		{
			RaiseOnHierarchyMoveChild(object);
		}
		if (ImGui::MenuItem("SetMoveParent"))
		{
			RaiseOnHierarchyMoveParent(object);
		}
		
		if (ImGui::MenuItem("Rename"))
		{
			OnHierarchyRenameClick(object);
		}

		ImGui::EndPopup();
	}
	if (open)
	{
		for (int i = 0; i < object->childObjects.size(); i++)
		{
			ModelTraversal(object->childObjects[i]);
		}
		ImGui::TreePop();
	}
}
