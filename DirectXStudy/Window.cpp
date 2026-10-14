#include "Window.h"
#include <tchar.h>
#include "DirectXMain.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "Log.h"
#include "Input.h"

Window* Window::instance = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}
	switch (message)
	{
	case WM_KEYDOWN:
		Instance()->GetInput()->SetKeyState(wParam,true);
		break;
	case WM_KEYUP:
		Instance()->GetInput()->SetKeyState(wParam, false);
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		Instance()->GetInput()->SetMouseButton(0,true);
		break;
	case WM_LBUTTONUP:
		Instance()->GetInput()->SetMouseButton(0, false);
		break;
	case WM_RBUTTONDOWN:
		Instance()->GetInput()->SetMouseButton(1, true);
		break;
	case WM_RBUTTONUP:
		Instance()->GetInput()->SetMouseButton(1, false);
		break;
	case WM_MBUTTONDOWN:
		Instance()->GetInput()->SetMouseButton(2, true);
		break;
	case WM_MBUTTONUP:
		Instance()->GetInput()->SetMouseButton(2, false);
		break;
	case WM_MOUSEWHEEL:
	{
		int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		Instance()->GetInput()->SetMouseWheelDelta(delta);
		break;
	}
	case WM_MOUSEMOVE:
	{
		Instance()->GetInput()->SetMousePos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

Window::Window(Input* input) : hInst(GetModuleHandle(nullptr))
{
	this->input = input;
	instance = this;
	WNDCLASSEX wces = {};
	wces.cbSize = sizeof(wces);
	wces.lpfnWndProc = WndProc;
	wces.hInstance = hInst;
	wces.lpszClassName = L"DirectXWindowClass";
	wces.hCursor = LoadCursor(nullptr, IDC_ARROW);

	if (!RegisterClassEx(&wces))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Windows Desktop Guided Tour"), NULL);

		return;
	}

	hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		L"DirectXWindowClass",
		L"Drirect11Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1280, 960,
		NULL,
		NULL,
		hInst,
		NULL);
	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindowEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return;
	}
	ImGui_ImplWin32_Init(hWnd);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
}
