#pragma once
#include <cstdint>

class Object;
class Component;

enum class EventType {
	SelectObject,
	SelectObjectID,
	RequestDeleteObject,
	ObjectWillBeDeleted,
	ActiveSceneChanged,
	ComponentAdded
};
enum class ActiveSceneMode {
	SceneMode,
	RuntimeMode,
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
class ActiveSceneChangedEventData : public EventData
{
public:
	ActiveSceneMode activeSceneMode;
};
class ComponentEventData : public EventData
{
public:
	Component* component;
};