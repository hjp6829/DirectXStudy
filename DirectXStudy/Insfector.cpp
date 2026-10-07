#include "Insfector.h"
#include "imgui_impl_win32.h"
#include "ModelCreater.h"
#include "Object.h"
#include "Hierarchy.h"
#include "Mesh.h"
#include "Log.h"

Insfector::Insfector()
{

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
