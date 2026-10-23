#pragma once
#include "DirectXMain.h"
#include "Window.h"
#include <chrono>
#include <queue>
#include <functional>
#include "EventDefinitions.h"

class ModelCreater;
class Mouse;
class UIManager;
class EventData;
class WorldManager;
class RendererManager;

class App {
	public:
		App();
		void Run();
private:
		DirectXMain* dxdMain;
		Window* window;
		Mouse* mouse;
		Input* input;
		UIManager* uimanager;
		WorldManager* worldManager;

private:
	std::queue<std::function<void()>> commandQueue;
	void RegisterEventHandlers();
	void DeleteObject(EventData* eventData);
	void ActiveSceneModeChange(EventData* eventData);
	ActiveSceneMode activeSceneMode = ActiveSceneMode::SceneMode;

};