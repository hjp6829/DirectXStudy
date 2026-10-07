#pragma once
#include <vector>
#include <d3d11.h>
#include <string>
#include "CameraObject.h"
#include "LightObject.h"

class Object;
class ModelCreater;
struct JsonSceneObjectData;

class SceneManager {
public:
	SceneManager(ID3D11Device* device);
	std::vector<Object*>* GetSceneObjects() { return &objects; }
	void SetKeyInput(int key, bool value);
	void SetKeyInputHold(int key);
	void CreateObjectSelect(std::string path);
	void DeleteModel(Object* object);
	void TestSaveMoveChild(Object* object){ testMoveChild  = object;}
	void TestSaveMoveParent(Object* object);
	void SetMouseRightDown(bool value) { camera->SetMouseRightValue(value); }
	void SetMouseWheelDown(bool value) { camera->SetMouseWheelDown(value); }
	void SetMouseWheelDelta(float delta) { camera->SetMouseWheelDelta(delta); }
	void SetMouseDeltaPos(int x, int y) { camera->SetMouseDeltaPos(x, y); }
	void SetCurrentSelectObject(Object* object) { currentSelectObject = object; }
	CameraObject* GetCamera() { return camera; }
	LightObject* GetLight() { return light; }
private:
	std::vector<Object*> objects;
	std::vector<uint64_t> objectIDs;
	ModelCreater* modelCreater;
	uint64_t objectID = 0;
	bool isCtrl;
	Object* testMoveChild;
	Object* testMoveParent;
	CameraObject* camera;
	LightObject* light;
	bool isNoSaveFile;
	Object* currentSelectObject;
	Object* currentDuplicateObject;
private:
	void SaveScene();
	void LoadSaveSceneFile();
	void SaveSceneObjectData(Object* object, nlohmann::json& jsonfile);
	void DeleteChiledModels(Object* object);
	void RegisterModelHierarchy(Object* object);
	Object* CreateSceneObject(const nlohmann::json& jsonfile, int type, std::filesystem::path path);
	void CreateInitJsonFile();
	void DuplicateObjectSave();
	void DuplicateObject();
	Object* DuplicateSceneObject(Object* object);
};