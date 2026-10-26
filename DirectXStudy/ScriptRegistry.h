#pragma once
#include <filesystem>
#include <functional>

class Component;
class Object;

class ScriptRegistry
{
public:
    ScriptRegistry();
    using ComponentFactory = std::function<std::unique_ptr<Component>()>;

    void RegisterScript( const std::string& name, ComponentFactory factory)
    {
        scriptFactories[name] = std::move(factory);
    }

    Component* CreateScript(const std::string& name,Object* object);
private:
    std::unordered_map<std::string, ComponentFactory> scriptFactories;
};