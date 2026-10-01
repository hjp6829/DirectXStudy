#pragma once
#include <Windows.h>
#include <functional>	

class Mouse {
public:
		int GetPosX() const { return pos.x; }
		int GetPosY() const { return pos.y; }
		void SetPos(int x, int y);

		void SetWheelDown(bool isDown){ OnMouseWheelDown(isDown); }
		void SetWheelDelta(float delta) {OnMouseWheelDelta(delta);}
		void SetMouseRight(bool value){ OnMouseRightDown(value); }
public:
		std::function<void(int, int)> OnMouseDeltaPos;
		std::function<void(bool)> OnMouseWheelDown;
		std::function<void(float)> OnMouseWheelDelta;
		std::function<void(bool)> OnMouseRightDown;
private:
	POINT pos;
	POINT prePos;
	POINT deltaPos = { 0, 0 };
};