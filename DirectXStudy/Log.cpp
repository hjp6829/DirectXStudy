#include "Log.h"

Log::Log()
{
	AllocConsole();

	FILE* file;
	freopen_s(&file, "CONOUT$", "w", stdout);
	freopen_s(&file, "CONOUT$", "w", stderr);
}

Log::~Log()
{
	FreeConsole();
}
