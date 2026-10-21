#include "ScriptRegistry.h"
#include "TestScript.h"
#include "CameraComponent.h"


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
}
