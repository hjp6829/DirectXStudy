#include "ScriptRegistry.h"
#include "TestScript.h"
#include "CameraComponent.h"
#include "Object.h"
#include "MeshRenderer.h"
#include "EventBus.h"
#include "EventDefinitions.h"
#include "SceneCameraComponent.h"
#include "LightComponent.h"
ScriptRegistry::ScriptRegistry()
{
    RegisterScript("TestScript",[]() {
            return std::make_unique<TestScript>();
        }
    );
    RegisterScript("CameraComponent", []() {
        return std::make_unique<CameraComponent>();
        }
    );
    RegisterScript("MeshRenderer", []() {
        return std::make_unique<MeshRenderer>();
        }
    );
    RegisterScript("SceneCameraComponent", []() {
        return std::make_unique<SceneCameraComponent>();
        }
    );
    RegisterScript("LightComponent", []() {
        return std::make_unique<LightComponent>();
        }
    );
}

Component* ScriptRegistry::CreateScript(const std::string& name, Object* object)
{
    auto it = scriptFactories.find(name);

    if (it == scriptFactories.end())
        return nullptr;

    std::unique_ptr<Component> TempComp = it->second();
    ComponentEventData componentEventData;
    componentEventData.component = TempComp.get();
    object->SetComponent(std::move(TempComp));
    EventBus::Publish(EventType::ComponentAdded, &componentEventData);
    return componentEventData.component;
}
