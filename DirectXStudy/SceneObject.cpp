#include "SceneObject.h"
#include "SceneSerializationData.h"

void SceneObject::SetMaterialIDX(int meshIDX, int MaterialIDX)
{
	//currentMeshs[meshIDX]->SetMaterialIDX(MaterialIDX);
}

void SceneObject::Serialize(nlohmann::json& j)
{
	Object::Serialize(j);
	j["ObjectType"] = ObjectSaveType::GameObject;
}

void SceneObject::Deserialize(const nlohmann::json& j)
{
}
