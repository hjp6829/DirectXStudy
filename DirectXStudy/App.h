#pragma once
#include "DirectXMain.h"
#include "Window.h"
#include <chrono>
#include "Camera.h"
#include "Light.h"
#include "Log.h"

class AssimpConverter;
class Mouse;
class UIManager;

class App {
	public:
		App();
		void Run();
	private:
		DirectXMain* dxdMain;
		Window* window;
		Camera* cam;
		Light* light;
		AssimpConverter* assimp;
		Mouse* mouse;
		Keyboard* keyboard;
		UIManager* uimanager;
};