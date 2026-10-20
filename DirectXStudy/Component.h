#pragma once
#include <string>
#include <memory>
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
protected:
	Object* currentObject;
	std::string componentName;
private:
	void DeleteComponentFromObject();
	bool isInit;
};