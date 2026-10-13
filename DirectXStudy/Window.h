#pragma once
#include <windows.h>
#include <windowsx.h>
#include "Mouse.h"

class Input;

class Window {
public:
	Window(Mouse* mouse, Input* input);
	~Window();
	static Window* Instance() { return instance; }
	Mouse* GetMouse() { return mouse; }
	Input* GetInput() { return input; }
	HWND GetHwnd() { return hWnd; }
private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
		HINSTANCE hInst;
		static Window* instance;
		Mouse* mouse;
		Input* input;
		HWND hWnd;
};