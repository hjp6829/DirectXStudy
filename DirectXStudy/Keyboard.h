#pragma once
#include <iostream>
#include <array>
#include <functional>

class Keyboard
{
public:
	void Update();
	void SetKeyState(int idx,bool value);
	std::function<void(int,bool)> OnKeyPressed;
	std::function<void(int)> OnKeyHold;
private:
	std::array<bool,256> currentState{};
	std::array<bool, 256> previousState{};

};