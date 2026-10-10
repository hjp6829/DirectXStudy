#include "Hierarchy.h"
#include "imgui_impl_win32.h"
#include "Object.h"
#include "Log.h"

Hierarchy::Hierarchy(std::vector<Object*>* modelContainer)
	: modelContainer(modelContainer)
{
}

void Hierarchy::UpdateUI()
{
    if (ImGui::Begin("Hierarchy"))
    {
        if (modelContainer != nullptr)
        {
            for (Object* object : *modelContainer)
            {
                if (object != nullptr)
                {
                    ModelTraversal(object);
                }
            }

            // 모든 트리 항목 아래에 빈 공간 드롭 영역 생성
            ImVec2 available = ImGui::GetContentRegionAvail();

            // 목록이 창을 채워도 아래쪽에 드롭할 공간 확보
            if (available.x < 1.0f)
                available.x = 1.0f;

            if (available.y < 30.0f)
                available.y = 30.0f;

            ImGui::Dummy(available);

            // 빈 공간에 드롭하면 부모를 해제하고 루트로 이동
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload =
                    ImGui::AcceptDragDropPayload("HIERARCHY_OBJECT"))
                {
                    if (payload->DataSize == sizeof(Object*))
                    {
                        Object* droppedObject =
                            *static_cast<Object* const*>(payload->Data);

                        if (droppedObject != nullptr)
                        {
                            OnHierarchyDrop(nullptr, droppedObject);
                        }
                    }
                }

                ImGui::EndDragDropTarget();
            }
        }
    }

    ImGui::End();
}

void Hierarchy::ModelTraversal(Object* object)
{
    if (object == nullptr)
        return;

    const bool isLeaf = object->childObjects.empty();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

    if (isLeaf)
    {
        flags |= ImGuiTreeNodeFlags_Leaf
            | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }
    if (object == currentSelectObject)
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }


    const bool open = ImGui::TreeNodeEx(
        static_cast<void*>(object),
        flags,
        "%s",
        object->objectName.c_str()
    );

    // 오브젝트 선택
    if (ImGui::IsItemClicked())
    {
        OnHierarchyClick(object);
		currentSelectObject = object;
    }

    // 드래그 시작
    if (ImGui::BeginDragDropSource())
    {
        Object* dragObject = object;

        ImGui::SetDragDropPayload(
            "HIERARCHY_OBJECT",
            &dragObject,
            sizeof(Object*)
        );

        ImGui::Text("%s", object->objectName.c_str());

        ImGui::EndDragDropSource();
    }

    // 오브젝트에 드롭하면 해당 오브젝트의 자식으로 이동
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload("HIERARCHY_OBJECT"))
        {
            if (payload->DataSize == sizeof(Object*))
            {
                Object* droppedObject =
                    *static_cast<Object* const*>(payload->Data);

                if (droppedObject != nullptr && droppedObject != object)
                {
                    OnHierarchyDrop(object, droppedObject);
                }
            }
        }

        ImGui::EndDragDropTarget();
    }

    // 트리 항목에 연결되는 우클릭 메뉴
    if (ImGui::BeginPopupContextItem())
    {
        OnHierarchyClick(object);
        currentSelectObject = object;

        if (ImGui::MenuItem("Delete"))
        {
            OnHierarchyDeleteClick(object);
        }

        if (ImGui::MenuItem("Rename"))
        {
            OnHierarchyRenameClick(object);
        }

        ImGui::EndPopup();
    }

    // 펼쳐진 노드의 자식 표시
    if (open && !isLeaf)
    {
        for (Object* child : object->childObjects)
        {
            ModelTraversal(child);
        }

        ImGui::TreePop();
    }
}