#include "Texture.h"

Texture::Texture(ComPtr<ID3D11ShaderResourceView> View,int idx)
{
	this->index=idx;
	/*D3D11_TEXTURE2D_DESC tDc ={};
	tDc.Width= surface.GetWidth();
	tDc.Height= surface.GetHeight();
	tDc.MipLevels=1;
	tDc.ArraySize=1;
	tDc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	tDc.SampleDesc.Count=1;
	tDc.SampleDesc.Quality =0;
	tDc.Usage= D3D11_USAGE_DEFAULT;
	tDc.BindFlags= D3D11_BIND_SHADER_RESOURCE;
	tDc.CPUAccessFlags=0;
	D3D11_SUBRESOURCE_DATA ts={};
	ts.pSysMem= surface.GetBufferPtr();
	ts.SysMemPitch = surface.GetWidth()* sizeof( Surface::Color);

	ComPtr<ID3D11Texture2D> pTexture2D;
	GetDevice(dxdMain)->CreateTexture2D(&tDc,&ts, pTexture2D.GetAddressOf());
	GetDevice(dxdMain)->CreateShaderResourceView(pTexture2D.Get(),NULL,textureView.GetAddressOf());*/
	textureView= View;
}

void Texture::ChangeNewTextureView(ComPtr<ID3D11ShaderResourceView> View)
{
	textureView = View;
}

void Texture::Bind(ID3D11DeviceContext* context)
{
	context->PSSetShaderResources(index,1, textureView.GetAddressOf());//2�� �Ű������� �󸶳� ���� view�� �ѹ��� �ѱ������ ���Ѱ�
	//���� ��������� �迭�� �ѱ�
}

