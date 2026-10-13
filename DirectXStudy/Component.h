#pragma once

class Object;

class Component {
public:
	Component(){};
	virtual ~Component() = default;
	virtual void  Start(){};
	virtual void Update(){};
	void Initialize(Object* object);
protected:
	Object* currentObject;
};