#include "ModelAsset.h"
#include "ModelNode.h"

ModelNode* ModelAsset::GetModelNode(uint64_t id)
{
    auto it = modelNodesDic.find(id);

    if (it != modelNodesDic.end())
    {
        ModelNode* modelNode = it->second;
        return modelNode;
    }

    return nullptr;
    return nullptr;
}
