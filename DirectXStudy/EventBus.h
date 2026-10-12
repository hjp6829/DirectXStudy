#pragma once
#include "EventDefinitions.h"
#include <functional>
#include <vector>

class EventBus {
	public:
	static void Subscribe(EventType type,std::function<void(EventData*)> callback);
	static void Publish(EventType type, EventData* data);
	private:
		inline static std::unordered_map<EventType,std::vector<std::function<void(EventData*)>>> eventListeners;
};