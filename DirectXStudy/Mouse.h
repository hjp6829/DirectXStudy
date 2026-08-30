#pragma once
#include <Windows.h>

class Mouse {
	public:
		int GetPosX() const { return pos.x; }
		int GetPosY() const { return pos.y; }
		void SetPos(int x, int y);

		bool IsMouseWheelDown(){return wheelDown;}
		void SetWheelDown(bool isDown){ wheelDown = isDown; }
		POINT GetMouseDeltaPos() { return deltaPos;}
		void SetWheelDelta(float delta);
		float GetWheelDelta(){return mouseWheelDelta;}
		void SetMouseRight(bool value){ rightDown = value;}
		bool GetMouseRight() {return rightDown;}
	private:
	POINT pos;
	POINT prePos;
	POINT deltaPos = { 0, 0 };
	bool wheelDown;
	float mouseWheelDelta;
	bool rightDown;
};