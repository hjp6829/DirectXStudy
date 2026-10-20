#include "Hierarchy.h"
#include "imgui_impl_win32.h"
#include "Object.h"
#include "Log.h"
#include "EventBus.h"
#include "EventDefinitions.h"
#include "WorldManager.h"

Hierarchy::Hierarchy(WorldManager* worldManager)
{
    this->worldManager = worldManager;
    RegisterEventHandlers();
}

void Hierarchy::UpdateUI()
{
    currentObjects = worldManager->GetRootObjects();
    if (ImGui::Begin("Hierarchy"))
    {
        if (currentObjects != nullptr)
        {
            for (auto& [key, value] : *currentObjects)
            {
                if (value != nullptr)
                {
                    ModelTraversal(value);
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
        SelectObjectData selectObjectData;
        selectObjectData.selectObject = object;
        EventBus::Publish(EventType::SelectObject, &selectObjectData);
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
        SelectObjectData selectObjectData;
        selectObjectData.selectObject = object;
        EventBus::Publish(EventType::SelectObject, &selectObjectData);

        if (ImGui::MenuItem("Delete"))
        {
            SelectObjectIDData selectObjectData;
            selectObjectData.objectID = object->objectID;
            EventBus::Publish(EventType::RequestDeleteObject, &selectObjectData);
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

void Hierarchy::RegisterEventHandlers()
{
    EventBus::Subscribe(EventType::SelectObject, [this](EventData* data){SetSelectObject(data);});
    EventBus::Subscribe(EventType::ObjectWillBeDeleted, [this](EventData* data) {DeleteCurrentObject(data); });
}

void Hierarchy::SetSelectObject(EventData* eventData)
{
    SelectObjectData* eventDataCast = static_cast<SelectObjectData*>(eventData);

    if (eventDataCast != nullptr)
    {
        currentSelectObject = eventDataCast->selectObject;
    }
}

void Hierarchy::DeleteCurrentObject(EventData* eventData)
{
    currentSelectObject = nullptr;
}

