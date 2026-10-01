#pragma once
#include <vector>
#include <d3d11.h>
#include <string>

class Object;
class ModelCreater;
struct JsonSceneObjectData;
#include "CameraObject.h"

class SceneManager {
public:
	SceneManager(ID3D11Device* device);
	std::vector<Object*>* GetSceneObjects() { return &objects; }
	void SetKeyInput(int key, bool value);
	void SetKeyInputHold(int key);
	void ModelSelected(std::string path);
	void DeleteModel(Object* object);
	void TestSaveMoveChild(Object* object){ testMoveChild  = object;}
	void TestSaveMoveParent(Object* object);
	void SetMouseRightDown(bool value) { camera->SetMouseRightValue(value); }
	void SetMouseWheelDown(bool value) { camera->SetMouseWheelDown(value); }
	void SetMouseWheelDelta(float delta) { camera->SetMouseWheelDelta(delta); }
	void SetMouseDeltaPos(int x, int y) { camera->SetMouseDeltaPos(x, y); }
	CameraObject* GetCamera() { return camera; }
private:
	std::vector<Object*> objects;
	std::vector<uint64_t> objectIDs;
	ModelCreater* modelCreater;
	uint64_t objectID = 0;
	bool isCtrl;
	Object* testMoveChild;
	Object* testMoveParent;
	CameraObject* camera;
private:
	void SaveScene();
	void LoadSaveSceneFile();
	void SaveSceneObjectData(Object* object, std::vector<JsonSceneObjectData>& jsonSceneObjectDatas);
	void DeleteChiledModels(Object* object);
	void RegisterModelHierarchy(Object* object);
};