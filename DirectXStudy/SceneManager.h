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
	std::vector<SceneObject*>* GetSceneObjects() { return &models; }
	void SetKeyInput(int key, bool value);
	void ModelSelected(std::string path);
	void DeleteModel(SceneObject* model);
private:
	std::vector<SceneObject*> models;
	std::vector<uint64_t> modelIDs;
	ModelCreater* modelCreater;
private:
	void SaveScene();
	void LoadSaveSceneFile();
	void SaveSceneObjectData(SceneObject* Model, std::vector<JsonSceneObjectData>& jsonSceneObjectDatas);
	void DeleteChiledModels(SceneObject* model);
	void RegisterModelHierarchy(SceneObject* model);
	uint64_t objectID = 0;
};