#pragma once
#include <cstdint>

class Object;

enum class EventType {
	SelectObject,
	SelectObjectID,
	RequestDeleteObject,
	ObjectWillBeDeleted
};

class EventData {
public:
	virtual ~EventData() = default;
};

class SelectObjectData : public EventData {
public:
	Object* selectObject;
};
class SelectObjectIDData : public EventData {
public:
	uint64_t objectID;
};