#include "AssimpConverter.h"
#include "Log.h"
#include "Surface.h"
#include "DirectXMain.h"

AssimpConverter::AssimpConverter()
{
	importer = new Assimp::Importer;
	scene = NULL;

}

AssimpConverter::~AssimpConverter()
{
}

void AssimpConverter::ReadAssetFile(std::wstring path)
{
	auto p = std::filesystem::path(path);
	curretnPath = p.string();
	assert(std::filesystem::exists(p));

	auto start = std::chrono::high_resolution_clock::now();

	scene = importer->ReadFile(p.string(), 0);

	auto readEnd = std::chrono::high_resolution_clock::now();

	scene = importer->ApplyPostProcessing(
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals
	);

	auto processEnd = std::chrono::high_resolution_clock::now();
	ReadModel();
	ReadMaterial(p);
	aiMatrix4x4 transform;
	ModelLoadData* model = new ModelLoadData();
	model->currentNode = scene->mRootNode;
	ParseNode(model, transform);
	LoadComplite(this);
}

void AssimpConverter::ParseNode(ModelLoadData* modelData, const aiMatrix4x4& parentTransform)
{
	aiMatrix4x4 localTransform = modelData->currentNode->mTransformation;
	aiMatrix4x4 worldTransform = parentTransform * localTransform;

	aiNode* currentNode = modelData->currentNode;

	modelData->mat = DirectX::XMMatrixTranspose(DirectX::XMMatrixSet(
		worldTransform.a1, worldTransform.a2, worldTransform.a3, worldTransform.a4,
		worldTransform.b1, worldTransform.b2, worldTransform.b3, worldTransform.b4,
		worldTransform.c1, worldTransform.c2, worldTransform.c3, worldTransform.c4,
		worldTransform.d1, worldTransform.d2, worldTransform.d3, worldTransform.d4
	));

	std::string nodeName = modelData->currentNode->mName.C_Str();
	modelData->modelName = nodeName;

	for (int i = 0; i < modelData->currentNode->mNumMeshes; i++)
	{
		int meshIndex = modelData->currentNode->mMeshes[i];
		modelData->meshIDX.push_back(meshIndex);
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

void AssimpConverter::ReadMaterial(std::filesystem::path modelPath)
{
	int materialNum = scene->mNumMaterials;

	for (int i = 0; i < materialNum; i++)
	{
		asMaterial material;
		aiMaterial* mat = scene->mMaterials[i];
		material.name = mat->GetName().C_Str();
		aiString texturePath;

		if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
		{
			std::filesystem::path path = texturePath.C_Str();
			std::filesystem::path texturePath = modelPath.parent_path() / L"textures" / path.filename();
			Surface surface = Surface::FromFile(texturePath);

			D3D11_TEXTURE2D_DESC tDc = {};
			tDc.Width = surface.GetWidth();
			tDc.Height = surface.GetHeight();
			tDc.MipLevels = 1;
			tDc.ArraySize = 1;
			tDc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			tDc.SampleDesc.Count = 1;
			tDc.SampleDesc.Quality = 0;
			tDc.Usage = D3D11_USAGE_DEFAULT;
			tDc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			tDc.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA ts = {};
			ts.pSysMem = surface.GetBufferPtr();
			ts.SysMemPitch = surface.GetWidth() * sizeof(Surface::Color);

			ComPtr<ID3D11Texture2D> pTexture2D;
			pDevice->CreateTexture2D(&tDc, &ts, pTexture2D.GetAddressOf());
			pDevice->CreateShaderResourceView(pTexture2D.Get(), NULL, material.textureView.GetAddressOf());

		}
		if (mat->GetTexture(aiTextureType_NORMALS, 0, &texturePath, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
		{
			std::filesystem::path path = texturePath.C_Str();
			std::filesystem::path texturePath = modelPath.parent_path() / L"textures" / path.filename();
			Surface surface = Surface::FromFile(texturePath);
			D3D11_TEXTURE2D_DESC tDc = {};
			tDc.Width = surface.GetWidth();
			tDc.Height = surface.GetHeight();
			tDc.MipLevels = 1;
			tDc.ArraySize = 1;
			tDc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			tDc.SampleDesc.Count = 1;
			tDc.SampleDesc.Quality = 0;
			tDc.Usage = D3D11_USAGE_DEFAULT;
			tDc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			tDc.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA ts = {};
			ts.pSysMem = surface.GetBufferPtr();
			ts.SysMemPitch = surface.GetWidth() * sizeof(Surface::Color);

			ComPtr<ID3D11Texture2D> pTexture2D;
			pDevice->CreateTexture2D(&tDc, &ts, pTexture2D.GetAddressOf());
			pDevice->CreateShaderResourceView(pTexture2D.Get(), NULL, material.normalView.GetAddressOf());

		}
		materials.push_back(std::move(material));
	}
}

