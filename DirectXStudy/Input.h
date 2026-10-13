#pragma once
#include <iostream>
#include <array>
#include <functional>

class Input
{
public:
	void Update();
	void SetKeyState(int idx,bool value);
	static bool GetKey(int key);
	static bool GetKeyDown(int key);
	static bool GetKeyUp(int key);
private:
	static std::array<bool,256> currentState;
	static std::array<bool, 256> previousState;

};