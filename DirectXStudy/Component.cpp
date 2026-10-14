#include "Component.h"
#include "imgui_impl_win32.h"
#include "Object.h"

void Component::Initialize(Object* object)
{
	currentObject = object;
}

void Component::InspectorUpdate()
{
	ImGui::Text(componentName.c_str());
	ImGui::SameLine();
	if (ImGui::SmallButton("X"))
	{
		DeleteComponentFromObject();
	}
}

void Component::DeleteComponentFromObject()
{
	currentObject->DeleteComponent(this);
}
