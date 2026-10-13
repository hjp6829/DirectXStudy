#include "ScriptRegistry.h"
#include "TestScript.h"


ScriptRegistry::ScriptRegistry()
{
    RegisterScript("TestScript",[]() {
            return std::make_unique<TestScript>();
        }
    );
	
}
