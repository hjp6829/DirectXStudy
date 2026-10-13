#pragma once
#include <filesystem>
#include <functional>

class ScriptComponent;

class ScriptRegistry
{
public:
    ScriptRegistry();
    using ScriptFactory = std::function<std::unique_ptr<ScriptComponent>()>;

    void RegisterScript( const std::string& name,ScriptFactory factory)
    {
        scriptFactories[name] = std::move(factory);
    }

    std::unique_ptr<ScriptComponent> CreateScript( const std::string& name)
    {
        auto it = scriptFactories.find(name);

        if (it == scriptFactories.end())
            return nullptr;

        return it->second();
    }

private:
    std::unordered_map<std::string, ScriptFactory> scriptFactories;
};