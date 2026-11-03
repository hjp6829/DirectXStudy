#include "ModelAsset.h"
#include "ModelNode.h"

ModelNode* ModelAsset::GetModelNode(std::string guid)
{
    auto it = modelNodesDic.find(guid);

    if (it != modelNodesDic.end())
    {
        ModelNode* modelNode = it->second;
        return modelNode;
    }

    return nullptr;
}
