#pragma once
#include <windows.h>
#include <windowsx.h>
#include "Mouse.h"

class Keyboard;

class Window {
public:
	Window(Mouse* mouse, Keyboard* keybard);
	~Window();
	static Window* Instance() { return instance; }
	Mouse* GetMouse() { return mouse; }
	Keyboard* GetKeyboard() { return keyboard; }
	HWND GetHwnd() { return hWnd; }
private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
		HINSTANCE hInst;
		static Window* instance;
		Mouse* mouse;
		Keyboard* keyboard;
		HWND hWnd;
};