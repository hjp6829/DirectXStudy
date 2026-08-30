#pragma once
#include <filesystem>
#include <cassert>
#include <utility>
#include "Datas.h"
#include <functional>

//#ifdef _DEBUG
//#pragma comment(lib, "Assimp/assimp-vc143-mtd.lib")
//#else
//#pragma comment(lib, "Assimp/assimp-vc143-mt.lib")
//#endif
class DirectXMain;

class AssimpConverter {
public:
	AssimpConverter();
	~AssimpConverter();
	void ReadAssetFile(std::wstring path, DirectXMain* dxd);
	std::vector<ModelLoadData*>& GetModelData() { return models; }
	asMesh* GetMeshData(unsigned int idx) { return &meshs[idx]; }
	asMaterial* GetMaterial(unsigned int idx) { return &materials[idx]; }
	std::function<void(AssimpConverter*)> LoadComplite;
private:
	Assimp::Importer* importer;
	const aiScene* scene;
	std::vector<asMesh> meshs;
	std::vector<asMaterial> materials;
	std::vector<ModelLoadData*> models;
	DirectXMain* dxdMain;
	int modelIDX = 0;
private:
	void ParseNode(ModelLoadData* modelData, const aiMatrix4x4& parentTransform);
	void ReadModel();
	void ReadMaterial(std::filesystem::path modelPath);
};