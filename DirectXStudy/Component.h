#pragma once
#include <string>
#include <memory>
#include <nlohmann/json.hpp>

class Object;

class Component {
public:
	Component(){};
	
	void Initialize(Object* object);
public:
	virtual ~Component() = default;
	virtual void  Awake(){};
	virtual void  Start(){};
	virtual void Update();
	virtual void LateUpdate();
	virtual void InspectorUpdate();
	virtual std::unique_ptr<Component> Clone() const = 0;
	virtual void Serialize(nlohmann::json& j) = 0;
	virtual void Deserialize(const nlohmann::json& j) = 0;
	std::string GetComponentName() {return componentName;}
	Object* GetOwnObject(){return currentObject;}
protected:
	Object* currentObject;
	std::string componentName;
private:
	void DeleteComponentFromObject();
	bool isInit;
};