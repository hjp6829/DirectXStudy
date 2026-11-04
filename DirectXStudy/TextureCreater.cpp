#include "TextureCreater.h"
#include "Surface.h"

TextureCreater::TextureCreater(ID3D11Device* device)
{	
	this->device = device;
}

ComPtr<ID3D11Texture2D> TextureCreater::CreateTextureFromFile(const std::wstring& texturePath)
{
	Surface surface = Surface::FromFile(texturePath);

	D3D11_TEXTURE2D_DESC tDc = {};
	tDc.Width = surface.GetWidth();
	tDc.Height = surface.GetHeight();
	tDc.MipLevels = 1;
	tDc.ArraySize = 1;
	tDc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	tDc.SampleDesc.Count = 1;
	tDc.SampleDesc.Quality = 0;
	tDc.Usage = D3D11_USAGE_DEFAULT;
	tDc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA ts = {};
	ts.pSysMem = surface.GetBufferPtr();
	ts.SysMemPitch = surface.GetWidth() * sizeof(Surface::Color);

	ComPtr<ID3D11Texture2D> pTexture2D;
	device->CreateTexture2D(&tDc, &ts, pTexture2D.GetAddressOf());
    return pTexture2D;
}
