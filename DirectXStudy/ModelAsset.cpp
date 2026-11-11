#include "ModelAsset.h"
#include "ModelNode.h"

ModelNode* ModelAsset::GetModelNode(uint64_t guid)
{
    auto it = modelNodesDic.find(guid);

    if (it != modelNodesDic.end())
    {
        ModelNode* modelNode = it->second;
        return modelNode;
    }

    return nullptr;
}

void ModelAsset::SetMaterialTexture(int matertialIDX, TextureAsset* textureAsset, aiTextureType type,std::string name)
{
    auto it = modelMaterialsDic.find(matertialIDX);
    Material* material = nullptr;
    if (it != modelMaterialsDic.end())
    {
        material = it->second;
    }
    else
    {
        material = new Material();
		material->name = name;
		modelMaterialsDic.insert({ matertialIDX, material });
    }
    if(type == aiTextureType::aiTextureType_DIFFUSE)
    {
        material->texture = textureAsset;
    }
    else if(type == aiTextureType::aiTextureType_NORMALS)
    {
        material->normal = textureAsset;
	}
}
