#include "Input.h"

std::array<bool, 256> Input::currentState{};
std::array<bool, 256> Input::previousState{};

void Input::Update()
{
	previousState = currentState;
}

void Input::SetKeyState(int idx, bool value)
{
	currentState[idx] = value;
}

bool Input::GetKey(int key)
{
	return currentState[key];
}
bool Input::GetKeyDown(int key)
{
	return currentState[key] && !previousState[key];
}
bool Input::GetKeyUp(int key)
{
	return !currentState[key] && previousState[key];
}
