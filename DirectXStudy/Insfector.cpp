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
	if(currentObject == nullptr)
		return;
	currentObject->ShowInspectorUI();
}

void Insfector::ObjectDelete()
{
	currentObject=nullptr;
}
