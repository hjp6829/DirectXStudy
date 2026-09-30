#pragma once
#include <vector>
#include <d3d11.h>
#include <string>

class Object;
class ModelCreater;
struct JsonSceneObjectData;

class SceneManager {
public:
	SceneManager(ID3D11Device* device);
	std::vector<Object*>* GetSceneObjects() { return &objects; }
	void SetKeyInput(int key, bool value);
	void ModelSelected(std::string path);
	void DeleteModel(Object* object);
	void TestSaveMoveChild(Object* object){ testMoveChild  = object;}
	void TestSaveMoveParent(Object* object);
private:
	std::vector<Object*> objects;
	std::vector<uint64_t> objectIDs;
	ModelCreater* modelCreater;
	uint64_t objectID = 0;
	bool isCtrl;
	Object* testMoveChild;
	Object* testMoveParent;
private:
	void SaveScene();
	void LoadSaveSceneFile();
	void SaveSceneObjectData(Object* object, std::vector<JsonSceneObjectData>& jsonSceneObjectDatas);
	void DeleteChiledModels(Object* object);
	void RegisterModelHierarchy(Object* object);
};