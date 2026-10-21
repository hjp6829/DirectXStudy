#pragma once
#include <filesystem>
#include <functional>

class Component;

class ScriptRegistry
{
public:
    ScriptRegistry();
    using ComponentFactory = std::function<std::unique_ptr<Component>()>;

    void RegisterScript( const std::string& name, ComponentFactory factory)
    {
        scriptFactories[name] = std::move(factory);
    }

    std::unique_ptr<Component> CreateScript( const std::string& name)
    {
        auto it = scriptFactories.find(name);

        if (it == scriptFactories.end())
            return nullptr;

        return it->second();
    }

private:
    std::unordered_map<std::string, ComponentFactory> scriptFactories;
};