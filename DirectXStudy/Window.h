#pragma once
#include <windows.h>
#include <windowsx.h>

class Input;

class Window {
public:
	Window( Input* input);
	~Window();
	static Window* Instance() { return instance; }
	Input* GetInput() { return input; }
	HWND GetHwnd() { return hWnd; }
private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
		HINSTANCE hInst;
		static Window* instance;
		Input* input;
		HWND hWnd;
};