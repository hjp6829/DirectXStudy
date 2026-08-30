#pragma once
#include <Windows.h>
#include <iostream>

class Log {
public:
	Log();
	~Log();
	static void PrintLog(std::string message) {
		std::cout << message << std::endl;
	}
};