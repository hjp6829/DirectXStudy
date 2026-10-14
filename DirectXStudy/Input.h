#pragma once
#include <iostream>
#include <array>
#include <functional>
#include <Windows.h>

class Input
{
public:
	void Update();
	void SetKeyState(int idx,bool value);
	void SetMouseButton(int button,bool value);
	void SetMousePos(int x, int y);
	void SetMouseWheelDelta(int delta);
public:
	static bool GetKey(int key);
	static bool GetKeyDown(int key);
	static bool GetKeyUp(int key);
	static bool GetMouseButton(int button);
	static bool GetMouseButtonDown(int button);
	static bool GetMouseButtonUp(int button);
	static POINT mousePositionDelta();
	static POINT mousePosition() { return mousePos; }
	static int mouseWheelDelta(){ return mouseWheelDeltaValue;}
private:
	static std::array<bool,256> currentKeyboardState;
	static std::array<bool, 256> previousKeyboardState;
	static std::array<bool, 3> currentMouseState;
	static std::array<bool, 3> previousMouseState;
	static POINT mousePos;
	POINT mousePrePos;
	static POINT mouseDeltaPos;
	static int mouseWheelDeltaValue;
};