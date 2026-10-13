#pragma once
#include "DirectXMain.h"
#include "Window.h"
#include <chrono>
#include "Log.h"
#include <queue>
#include <functional>

class ModelCreater;
class Mouse;
class UIManager;
class SceneManager;
struct JsonSceneObjectData;
class EventData;

class App {
	public:
		App();
		void Run();
private:
		DirectXMain* dxdMain;
		Window* window;
		Mouse* mouse;
		Keyboard* keyboard;
		UIManager* uimanager;
		SceneManager* sceneManager;
private:
	std::queue<std::function<void()>> commandQueue;
	void RegisterEventHandlers();
	void DeleteObject(EventData* eventData);
};