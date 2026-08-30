#include <windows.h>
#include "App.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	App{}.Run();
	return -1;
}
