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
struct JsonSceneModelData;

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
		SceneModel* testSaveModel;
	private:
		void ModelSelected(std::string modelName);
		void DeleteModel(SceneModel* model);
		void DeleteChiledModels(SceneModel* model);
		void SaveModel(SceneModel* model);
		void MoveModel(SceneModel* model);
		void RenameModel(SceneModel* model);
		void SaveScene(int key, bool value);
		void SaveSceneModelData(SceneModel* parentModel, std::vector<JsonSceneModelData>& jsonSceneModelDatas);
		void LoadSaveSceneFile(int key, bool value);
};