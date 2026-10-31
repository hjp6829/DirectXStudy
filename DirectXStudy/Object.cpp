#include "Object.h"
#include "DirectXMain.h"

#include "Log.h"
#include "imgui_impl_win32.h"
#include "Component.h"
#include <algorithm>

Object::~Object() = default;

Object::Object()
{
	std::unique_ptr<Transform> transformTemp = std::make_unique<Transform>();
	transform = transformTemp.get();
	components[transformTemp->GetComponentName()] = std::move(transformTemp);

	for (auto& [key, value] : components)
	{
		value->Awake();
	}
	for (auto& [key, value] : components)
	{
		value->Start();
	}
}

void Object::ShowComponentInspectorUI()
{	
	for (auto& [key, value] : components)
	{
		ImGui::Separator();
		value->InspectorUpdate();
	}
}
void Object::SetComponent(std::unique_ptr<Component> component)
{
	component->Initialize(this);
	component->Awake();
	component->Start();
	components[component->GetComponentName()] = std::move(component);
}
void Object::DeleteComponent(Component* component)
{
	commandQueue.push([this, component]()
		{
			components.erase(component->GetComponentName());
		});
}
void Object::UpdateComponent(float deltaTime)
{
	for (auto& [key, value] : components)
	{
		value->Update();
	}
}
void Object::LateUpateComponent(float deltaTime)
{
	for (auto& [key, value] : components)
	{
		value->LateUpdate();
	}
	while (!commandQueue.empty())
	{
		auto command = std::move(commandQueue.front());
		commandQueue.pop();

		command();
	}
}
void Object::Serialize(nlohmann::json& j)
{
	j = nlohmann::json{
		{"parentObjectID", parentObject == nullptr ? 0 : parentObject->objectID},
		{ "ObjectName", objectName},
		{ "objectID", objectID },
		{ "isRootObject", IsRootObject() == true ? 1 : 0},
	};
	j["Components"] = nlohmann::json::array();

	for (auto& [key, value] : components)
	{
		nlohmann::json componentJson;
		value->Serialize(componentJson);
		j["Components"].push_back(componentJson);
	}
}
void Object::Deserialize(const nlohmann::json& j)
{
	j.at("parentObjectID").get_to(parentobjectID);
	j.at("objectID").get_to(objectID);
	j.at("ObjectName").get_to(objectName);
	SetRootNodeCheck(j.at("isRootObject").get<float>() == 1 ? true : false);

	if (!j.contains("Components") || !j["Components"].is_array())
		return;

	for (const auto& componentJson : j["Components"])
	{
		std::string componentName = componentJson.at("ComponentName").get<std::string>();
		try
		{
			components[componentName]->Deserialize(componentJson);
		}
		catch (const nlohmann::json::out_of_range& e)
		{
			Log::PrintLog(e.what());
		}
	}
}
void Object::SetDataToCopy(Object* copyObject)
{
	//copyObject->GetTransform()->CopyTransform(transform);
	copyObject->SetRootNodeCheck(IsRootObject());
	copyObject->objectName = objectName;
	copyObject->objectID = objectID;
	copyObject->parentobjectID = parentobjectID;
	for (auto& [key, value] : components)
	{
		std::unique_ptr<Component> temp = value->Clone();

		if (Transform* transformComponent = dynamic_cast<Transform*>(value.get()))
		{
			copyObject->GetTransform()->CopyTransform(transformComponent);
			continue;
		}

		copyObject->SetComponent(std::move(temp));
	}
}

void Object::UpdateObject()
{
	transform->MakeLocalMatrix();
	if (parentObject != nullptr)
		transform->SetWorldMatrix(transform->GetLocalMatrix() * parentObject->GetTransform()->GetWorldMatrix());
	else
		transform->SetWorldMatrix(transform->GetLocalMatrix());
	for (int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->UpdateObject();
	}
}


void Object::RemoveModelData()
{
	if (parentObject)
	{
		parentObject->RemoveChildObject(this);
		parentObject = nullptr;
	}
	RemoveAllChileObject(this);
}

void Object::RemoveChildObject(Object* childObject)
{
	for (auto it = childObjects.begin(); it != childObjects.end(); ++it)
	{
		if (*it == childObject)
		{
			childObjects.erase(it);
			break;
		}
	}
}

void Object::RemoveAllChileObject(Object* childObject)
{
	for (int i = 0; i < childObject->childObjects.size(); i++)
	{
		RemoveAllChileObject(childObject->childObjects[i]);
	}
}

void Object::InsertChildSceneObject(Object* childObject)
{
	childObject->parentObject = this;
	childObjects.push_back(childObject);
}

void Object::RemoveFromParent()
{
	if(parentObject != nullptr)
		parentObject->RemoveChildObject(this);
}