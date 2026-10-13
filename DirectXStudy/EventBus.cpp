#include "EventBus.h"

void EventBus::Subscribe(EventType type, std::function<void(EventData*)> callback)
{
	eventListeners[type].push_back(callback);
}

void EventBus::Publish(EventType type, EventData* data)
{
	std::vector<std::function<void(EventData*)>> currentEvent = eventListeners[type];
	for (int i = 0; i < currentEvent.size(); i++)
	{
		currentEvent[i](data);
	}
}
