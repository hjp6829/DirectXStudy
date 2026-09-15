#pragma once
#include "DirectXMain.h"
#include "Window.h"
#include <chrono>
#include "Camera.h"
#include "Light.h"
#include "Log.h"

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
		Camera* cam;
		Light* light;
		Mouse* mouse;
		Keyboard* keyboard;
		UIManager* uimanager;
		SceneManager* sceneManager;
private:
};