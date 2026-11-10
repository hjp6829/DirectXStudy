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
	AssimpConverter(ID3D11Device* device);
	~AssimpConverter();
	void ReadAssetFile(std::filesystem::path path);
	std::vector<ModelLoadData*>& GetModelData() { return models; }
	asMesh* GetMeshData(unsigned int idx) { return &meshs[idx]; }
	std::function<void(AssimpConverter*)> LoadComplite;
	std::string GetCurrentPath() { return curretnPath; }
	std::vector<std::string> GetTexturePath(const aiNode* node);
	std::vector<TextureRequest> ReadTextureData(std::filesystem::path modelPath);
	ModelLoadData* CreateModelLoadData(const std::string& path);
private:
	Assimp::Importer* importer;
	const aiScene* scene;
	std::vector<asMesh> meshs;
	std::vector<ModelLoadData*> models;
	int objectIDX = 0;
	std::string curretnPath;
	ID3D11Device* device;
	std::vector<TextureRequest> texturePaths;
private:
	void ParseNode(ModelLoadData* modelData, const aiMatrix4x4& parentTransform);
	void ReadModel();
	std::string GetNodeLocalPath(const aiNode* node);
};