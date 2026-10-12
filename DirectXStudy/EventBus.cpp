#include "EventBus.h"

void EventBus::Subscribe(EventType type, std::function<void(EventData*)> callback)
{
	eventListeners[type].push_back(callback);
}

void EventBus::Publish(EventType type, EventData* data)
{
	for (auto& [key, value] : eventListeners)
	{
		for (int i = 0; i < value.size(); i++)
		{
			value[i](data);
		}
	}
}
