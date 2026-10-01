#pragma once
#include "DirectXMain.h"
#include "Window.h"
#include <chrono>
#include "Light.h"
#include "Log.h"
#include <queue>
#include <functional>

class ModelCreater;
class Mouse;
class UIManager;
class SceneManager;
struct JsonSceneObjectData;

class App {
	public:
		App();
		void Run();
private:
		DirectXMain* dxdMain;
		Window* window;
		Light* light;
		Mouse* mouse;
		Keyboard* keyboard;
		UIManager* uimanager;
		SceneManager* sceneManager;
private:
	std::queue<std::function<void()>> commandQueue;
};