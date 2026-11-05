#include "AssimpConverter.h"
#include "Log.h"
#include "Surface.h"
#include "DirectXMain.h"
#include "AssetManager.h"

AssimpConverter::AssimpConverter(ID3D11Device* device)
{
	importer = new Assimp::Importer;
	scene = NULL;
	this->device = device;
}

AssimpConverter::~AssimpConverter()
{
}

void AssimpConverter::ReadAssetFile(std::filesystem::path path)
{
	curretnPath = path.string();
	assert(std::filesystem::exists(path));

	auto start = std::chrono::high_resolution_clock::now();
	scene = importer->ReadFile(path.string(), 0);
	auto readEnd = std::chrono::high_resolution_clock::now();

	scene = importer->ApplyPostProcessing(
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals
	);

	auto processEnd = std::chrono::high_resolution_clock::now();
}

void AssimpConverter::ParseNode(ModelLoadData* modelData, const aiMatrix4x4& parentTransform)
{
	aiMatrix4x4 localTransform = modelData->currentNode->mTransformation;
	aiMatrix4x4 worldTransform = parentTransform * localTransform;

	aiNode* currentNode = modelData->currentNode;

	modelData->localPath = GetNodeLocalPath(currentNode);
	modelData->mat = DirectX::XMMatrixTranspose(DirectX::XMMatrixSet(
		worldTransform.a1, worldTransform.a2, worldTransform.a3, worldTransform.a4,
		worldTransform.b1, worldTransform.b2, worldTransform.b3, worldTransform.b4,
		worldTransform.c1, worldTransform.c2, worldTransform.c3, worldTransform.c4,
		worldTransform.d1, worldTransform.d2, worldTransform.d3, worldTransform.d4
	));

	aiVector3D scale;
	aiQuaternion rotation;
	aiVector3D position;

	currentNode->mTransformation.Decompose(scale, rotation, position);//���ʹϾ�

	std::string nodeName = modelData->currentNode->mName.C_Str();
	modelData->modelName = nodeName;
	modelData->localMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixSet(
		localTransform.a1, localTransform.a2, localTransform.a3, localTransform.a4,
		localTransform.b1, localTransform.b2, localTransform.b3, localTransform.b4,
		localTransform.c1, localTransform.c2, localTransform.c3, localTransform.c4,
		localTransform.d1, localTransform.d2, localTransform.d3, localTransform.d4
	));

	for (int i = 0; i < modelData->currentNode->mNumMeshes; i++)
	{
		int meshIndex = modelData->currentNode->mMeshes[i];
		modelData->meshs.push_back(&meshs[meshIndex]);
	}

	models.push_back(modelData);
	for (int i = 0; i < modelData->currentNode->mNumChildren; i++)
	{
		ModelLoadData* childModel = new ModelLoadData();
		childModel->currentNode = modelData->currentNode->mChildren[i];
		modelData->childNodes.push_back(childModel);
		ParseNode(childModel, worldTransform);
	}

}

void AssimpConverter::ReadModel()
{
	int meshNum = scene->mNumMeshes;

	for (int i = 0; i < meshNum; i++)
	{
		asMesh asmesh;
		aiMesh* mesh = scene->mMeshes[i];
		asmesh.origMesh = mesh;
		asmesh.meshName = mesh->mName.C_Str();
		asmesh.materialIDX = mesh->mMaterialIndex;

		for (int v = 0; v < mesh->mNumVertices; v++)
		{
			Vertex vertex;
			auto& pos = mesh->mVertices[v];
			vertex.position = DirectX::XMFLOAT3(pos.x, pos.y, pos.z);

			if (mesh->HasTextureCoords(0))
				vertex.uv = DirectX::XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
			if (mesh->HasNormals())
				vertex.normal = DirectX::XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
			if (mesh->HasTangentsAndBitangents())
				vertex.tangent = DirectX::XMFLOAT3(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z);

			asmesh.vertexs.push_back(vertex);
		}
		for (int d = 0; d < mesh->mNumFaces; d++)
		{
			aiFace& face = mesh->mFaces[d];
			for (int f = 0; f < face.mNumIndices; f++)
			{
				asmesh.Indexs.push_back(face.mIndices[f]);
			}
		}

		meshs.push_back(std::move(asmesh));
	}
}

std::vector<TextureRequest> AssimpConverter::ReadTextureData(std::filesystem::path modelPath)
{
	int materialNum = scene->mNumMaterials;
	texturePaths.clear();

	for (int i = 0; i < materialNum; i++)
	{
		aiMaterial* mat = scene->mMaterials[i];
		aiString texturePath;

		if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
		{
			TextureRequest request;
			request.materialIndex = i;
			request.type = aiTextureType_DIFFUSE;
			request.path = modelPath.parent_path() / L"textures" / std::filesystem::path(texturePath.C_Str()).filename();
			texturePaths.push_back(request);
		}
		if (mat->GetTexture(aiTextureType_NORMALS, 0, &texturePath, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
		{
			TextureRequest request;
			request.materialIndex = i;
			request.type = aiTextureType_NORMALS;
			request.path = modelPath.parent_path() / L"textures" / std::filesystem::path(texturePath.C_Str()).filename();
			texturePaths.push_back(request);
		}
	}
	return texturePaths;
}

ModelLoadData* AssimpConverter::CreateModelLoadData(const std::string& path)
{
	ReadModel();
	aiMatrix4x4 transform;
	ModelLoadData* model = new ModelLoadData();
	model->currentNode = scene->mRootNode;
	ParseNode(model, transform);
	return model;
}

std::string AssimpConverter::GetNodeLocalPath(const aiNode* node)
{
	if (node == nullptr)
		return "";

	std::string path = node->mName.C_Str();

	const aiNode* parent = node->mParent;

	while (parent != nullptr)
	{
		path = std::string(parent->mName.C_Str()) + "/" + path;
		parent = parent->mParent;
	}

	return path;
}

