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
	components.push_back(std::move(transformTemp));

	for (auto& component : components)
	{
		component->Awake();
	}
	for (auto& component : components)
	{
		component->Start();
	}
}

void Object::ShowComponentInspectorUI()
{	
	for (auto& component : components)
	{
		ImGui::Separator();
		component->InspectorUpdate();
	}
}
void Object::SetComponent(std::unique_ptr<Component> component)
{
	component->Initialize(this);
	component->Awake();
	component->Start();
	components.push_back(std::move(component));
}
void Object::DeleteComponent(Component* component)
{
	commandQueue.push([this, component]()
		{
			components.erase(
				std::remove_if(
					components.begin(),
					components.end(),
					[component](const std::unique_ptr<Component>& current)
					{
						return current.get() == component;
					}
				),
				components.end()
			);
		});
}
void Object::UpdateComponent(float deltaTime)
{
	for (auto& component : components)
	{
		component->Update();
	}
}
void Object::LateUpateComponent(float deltaTime)
{
	for (auto& component : components)
	{
		component->LateUpdate();
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
		{"ModelNamePath", modelNamePath},
		{ "TestModelNamePath", objectName},
		{ "objectID", objectID },
		{ "isRootObject", IsRootObject() == true ? 1 : 0},
	};
	j["Components"] = nlohmann::json::array();

	for (auto& component : components)
	{
		nlohmann::json componentJson;
		component->Serialize(componentJson);
		j["Components"].push_back(componentJson);
	}
}
void Object::Deserialize(const nlohmann::json& j)
{
	j.at("parentObjectID").get_to(parentobjectID);
	j.at("objectID").get_to(objectID);
	SetRootNodeCheck(j.at("isRootObject").get<float>() == 1 ? true : false);
	if (!j.contains("Components") || !j["Components"].is_array())
		return;

	for (const auto& componentJson : j["Components"])
	{
		std::string type = componentJson.value("type", std::string(""));

		if (type == "Transform")
		{
			transform->Deserialize(componentJson);
			continue;
		}
	}
}
void Object::SetDataToCopy(Object* copyObject)
{
	//copyObject->GetTransform()->CopyTransform(transform);
	copyObject->SetRootNodeCheck(IsRootObject());
	copyObject->objectName = objectName;
	copyObject->modelNamePath = modelNamePath;
	copyObject->objectID = objectID;
	copyObject->parentobjectID = parentobjectID;
	for (const auto& component : components)
	{
		std::unique_ptr<Component> temp = component->Clone();

		if (Transform* transformComponent = dynamic_cast<Transform*>(component.get()))
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
