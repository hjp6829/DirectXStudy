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
#include "RenderingTypes.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class Mesh;
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
	void RenderObject(RenderPass renderPass, ID3D11DeviceContext* context,DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);

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

	void SetTransform(Transform* transform) {this->transform = transform;}
	Transform* GetTransform(){return transform;}
	std::vector<std::unique_ptr<Component>>* GetComponents(){return &components;}
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