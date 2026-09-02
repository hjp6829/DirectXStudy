#pragma once
#include "DirectXMain.h"
#include "Window.h"
#include <chrono>
#include "Camera.h"
#include "Light.h"
#include "Log.h"
#include <vector>

class ModelCreater;
class Mouse;
class UIManager;
class SceneModel;

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
		std::vector<SceneModel*> models;
		ModelCreater* modelCreater;
	private:
		void ModelSelected(std::string modelName);
};