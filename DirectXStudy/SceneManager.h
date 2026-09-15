#pragma once
#include <vector>
#include <d3d11.h>
#include <string>

class SceneModel;
class ModelCreater;
struct JsonSceneModelData;

class SceneManager {
public:
	SceneManager(ID3D11Device* device);
	std::vector<SceneModel*>* GetSceneModels() { return &models; }
	void SetKeyInput(int key, bool value);
	void ModelSelected(std::string path);
	void DeleteModel(SceneModel* model);
private:
	std::vector<SceneModel*> models;
	std::vector<uint64_t> modelIDs;
	ModelCreater* modelCreater;
private:
	void SaveScene();
	void LoadSaveSceneFile();
	void SaveSceneModelData(SceneModel* Model, std::vector<JsonSceneModelData>& jsonSceneModelDatas);
	void DeleteChiledModels(SceneModel* model);
	void RegisterModelHierarchy(SceneModel* model);
	uint64_t objectID = 0;
};