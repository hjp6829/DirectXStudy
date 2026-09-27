#pragma once
#include <vector>
#include <d3d11.h>
#include <string>

class SceneObject;
class ModelCreater;
struct JsonSceneObjectData;

class SceneManager {
public:
	SceneManager(ID3D11Device* device);
	std::vector<SceneObject*>* GetSceneObjects() { return &objects; }
	void SetKeyInput(int key, bool value);
	void ModelSelected(std::string path);
	void DeleteModel(SceneObject* model);
	void TestSaveMoveChild(SceneObject* model){ testMoveChild  = model;}
	void TestSaveMoveParent(SceneObject* model);
private:
	std::vector<SceneObject*> objects;
	std::vector<uint64_t> objectIDs;
	ModelCreater* modelCreater;
	uint64_t objectID = 0;
	bool isCtrl;
	SceneObject* testMoveChild;
	SceneObject* testMoveParent;
private:
	void SaveScene();
	void LoadSaveSceneFile();
	void SaveSceneObjectData(SceneObject* Model, std::vector<JsonSceneObjectData>& jsonSceneObjectDatas);
	void DeleteChiledModels(SceneObject* model);
	void RegisterModelHierarchy(SceneObject* model);
};