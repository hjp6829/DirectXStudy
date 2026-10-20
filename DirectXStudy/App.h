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
class SceneManager;
class EventData;
class RuntimeManager;

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
		SceneManager* sceneManager;
		RuntimeManager* runtimeManager;
private:
	std::queue<std::function<void()>> commandQueue;
	void RegisterEventHandlers();
	void DeleteObject(EventData* eventData);
	void ActiveSceneModeChange(EventData* eventData);
	ActiveSceneMode activeSceneMode = ActiveSceneMode::SceneMode;
};