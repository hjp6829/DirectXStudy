#include "Input.h"

std::array<bool, 256> Input::currentKeyboardState{};
std::array<bool, 256> Input::previousKeyboardState{};
std::array<bool, 3> Input::currentMouseState;
std::array<bool, 3> Input::previousMouseState;
POINT Input::mouseDeltaPos;
POINT Input::mousePos;
int Input::mouseWheelDeltaValue;

void Input::Update()
{
	previousKeyboardState = currentKeyboardState;
	previousMouseState = currentMouseState;
	mouseWheelDeltaValue = 0;
	mouseDeltaPos.x = 0;
	mouseDeltaPos.y = 0;
}

void Input::SetKeyState(int idx, bool value)
{
	currentKeyboardState[idx] = value;
}

void Input::SetMouseButton(int button, bool value)
{
	currentMouseState[button] = value;
}
void Input::SetMouseWheelDelta(int delta)
{
	if (delta > 0)
		mouseWheelDeltaValue = 1;
	else if (delta < 0)
		mouseWheelDeltaValue = -1;
}
bool Input::GetKey(int key)
{
	return currentKeyboardState[key];
}
bool Input::GetKeyDown(int key)
{
	return currentKeyboardState[key] && !previousKeyboardState[key];
}
bool Input::GetKeyUp(int key)
{
	return !currentKeyboardState[key] && previousKeyboardState[key];
}

void Input::SetMousePos(int x, int y)
{
	mousePos.x = x;
	mousePos.y = y;
	int deltaPosXTemp = mousePos.x - mousePrePos.x;
	int deltaPosYTemp = mousePos.y - mousePrePos.y;
	if (deltaPosXTemp > 1 || deltaPosXTemp < -1)
		mouseDeltaPos.x = -deltaPosXTemp;
	else
		mouseDeltaPos.x = 0;
	if (deltaPosYTemp > 1 || deltaPosYTemp < -1)
		mouseDeltaPos.y = deltaPosYTemp;
	else
		mouseDeltaPos.y = 0;
	mousePrePos.x = x;
	mousePrePos.y = y;
}

bool Input::GetMouseButton(int button)
{
	return currentMouseState[button];
}

bool Input::GetMouseButtonDown(int button)
{
	return currentMouseState[button] && !previousMouseState[button];
}

bool Input::GetMouseButtonUp(int button)
{
	return !currentMouseState[button] && previousMouseState[button];
}

POINT Input::mousePositionDelta()
{
	return mouseDeltaPos;
}
