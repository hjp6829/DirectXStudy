#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

class TextureCreater
{
public:
	TextureCreater(ID3D11Device* device);
	ComPtr<ID3D11Texture2D> CreateTextureFromFile(const std::wstring& texturePath);
private:
	ID3D11Device* device;
};