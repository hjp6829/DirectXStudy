#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <memory>
#include <queue>
#include "Transform.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class Mesh;
class DirectXMain;
class ModelNode;
class Component;

class Object {
public:
	Object();
	virtual ~Object();
	Object* parentObject;
	ModelNode* currentModelNode;
	std::vector<Object*> childObjects;
	std::string objectName;
	std::string modelNamePath;
	uint64_t objectID;
	uint64_t parentobjectID;
	void RenderObject(DirectXMain* dxdMain);
	void OutlineRenderObject(DirectXMain* dxdMain);
	void PickingRenderObject(DirectXMain* dxdMain);

	void ToggleMeshEnable(bool value);
	bool meshEnable = true;
	bool IsRootObject() { return isRootObject; }
	void SetRootNodeCheck(bool value) { isRootObject = value; }
	void RemoveModelData();
	void RemoveChildObject(Object* childObject);
	void RemoveAllChileObject(Object* childObject);
	void InsertChildSceneObject(Object* childObject);
	void RemoveFromParent();

	void ShowComponentInspectorUI();
	void SetComponent(std::unique_ptr<Component> component);
	void DeleteComponent(Component* component);
	void UpdateComponent(float deltaTime);
	void LateUpateComponent(float deltaTime);

	Transform* GetTransform(){return transform;}
public:
	void virtual UpdateObject();
	void virtual Serialize(nlohmann::json& j);
	void virtual Deserialize(const nlohmann::json& j);
	void virtual SetDataToCopy(Object* copyObject);
protected:
	bool isRootObject;
	std::vector<std::unique_ptr<Component>> components;
	Transform* transform;
private:
	std::queue<std::function<void()>> commandQueue;
};