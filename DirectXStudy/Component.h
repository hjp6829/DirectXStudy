#pragma once
#include <string>
class Object;

class Component {
public:
	Component(){};
	
	void Initialize(Object* object);
public:
	virtual ~Component() = default;
	virtual void  Start() {};
	virtual void Update() {};
	virtual void InspectorUpdate();
protected:
	Object* currentObject;
	std::string componentName;
private:
	void DeleteComponentFromObject();
};