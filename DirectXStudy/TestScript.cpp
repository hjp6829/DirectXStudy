#include "TestScript.h"
#include "ScriptComponent.h"
#include "Object.h"
#include <DirectXMath.h>

void TestScript::Update()
{
	ScriptComponent::Update();
	DirectX::XMFLOAT3 position = currentObject->GetObjectPosition();
	position.x+=5;
	currentObject->SetPosition(position);
}

void TestScript::Start()
{
	ScriptComponent::Start();
}
