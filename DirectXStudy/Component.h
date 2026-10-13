#pragma once

class Component {
public:
	Component(){};
	virtual ~Component() = default;
	virtual void  Start(){};
	virtual void Update(){};
};