#pragma once
#include <Windows.h>
#include <iostream>
#include <DirectXMath.h>
#include <sstream>

class Log {
public:
	Log();
	~Log();
	static void PrintLog(std::string message) {
		std::cout << message << std::endl;
	}
    static void PrintMatrix(const DirectX::XMMATRIX& matrix)
    {
        DirectX::XMFLOAT4X4 m;
        DirectX::XMStoreFloat4x4(&m, matrix);

        for (int i = 0; i < 4; i++)
        {
            std::ostringstream oss;

            for (int j = 0; j < 4; j++)
            {
                oss << m.m[i][j] << "\t";
            }

            Log::PrintLog(oss.str());
        }
    }
};